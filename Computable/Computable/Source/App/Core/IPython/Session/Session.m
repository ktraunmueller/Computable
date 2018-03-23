//
//  Session.m
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 05.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Session.h"
#import "Message.h"
#import "AnsiParser.h"

#import <Python.h>
#import <zmq.h>
#import <zhelpers.h>

NSString *const ControlPortKey = @"control_port";
NSString *const HeartbeatPortKey = @"hb_port";
NSString *const IOPubPortKey = @"iopub_port";
NSString *const ShellPortKey = @"shell_port";
NSString *const StdinPortKey = @"stdin_port";
NSString *const IPAddressKey = @"ip";
NSString *const DigestKey = @"key";

static NSString *const ShellPortIdentify = @"shell";

static void *_zmqContext;

@implementation Session
{
    NSMutableArray *_requests;
    NSMutableArray *_pendingRequests;
    NSMutableArray *_responses;
    
    dispatch_queue_t _shell_queue;
    void *_shellPort;
    dispatch_queue_t _iopub_queue;
    void *_ioPort;
}

#pragma mark - Statics

+ (void)initialize
{
    _zmqContext = zmq_ctx_new();
}

#pragma mark - Session

- (id)initWithId:(NSString *)sessionId
   configuration:(NSDictionary *)configuration
{
    NSLog(@"+++ Session init +++");
    if (self = [super init])
    {
        _requests = [NSMutableArray array];
        _pendingRequests = [NSMutableArray array];
        _responses = [NSMutableArray array];
        _sessionId = sessionId;
        _configuration = configuration;
        _iopub_queue = dispatch_queue_create("IOPub dispatch queue", DISPATCH_QUEUE_SERIAL);
        _shell_queue = dispatch_queue_create("Shell dispatch queue", DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

- (void)dealloc
{
    NSLog(@"--- Session dealloc ---");
}

#pragma mark - ANSI Parsing

- (id)parseMessage:(char *)msg
{
    if (msg == NULL)
    {
        return nil;
    }
    NSString *plainText = [[NSString alloc] initWithUTF8String:msg];
    if (msg && plainText)
    {
        return plainText;
    }
    
    return [[AnsiParser new] parse:msg];
}

#pragma mark - Lifecycle

- (void)start
{
    if (_configuration == nil)
    {
        NSLog(@"Cannot start session, missing configuration");
        return;
    }
    
    NSString *ipAddr = [_configuration objectForKey:IPAddressKey];
    if (ipAddr == nil)
    {
        ipAddr = @"127.0.0.1";
    }
    
    // -- IOPub --
    dispatch_sync(_iopub_queue, ^{
        int iopubPort = [[_configuration objectForKey:IOPubPortKey] intValue];
        NSString *addr = [NSString stringWithFormat:@"tcp://%@:%d", ipAddr, iopubPort];
        
        // create subscriber socket
        _ioPort = zmq_socket(_zmqContext, ZMQ_SUB);
        // connect
        int rc = zmq_connect(_ioPort, [addr UTF8String]);
        assert(rc == 0);
        // subscribe/filter
        char *subscription = "";//"IOPub";
        rc = zmq_setsockopt(_ioPort, ZMQ_SUBSCRIBE, subscription, strlen(subscription));
        assert(rc == 0);
    });
    dispatch_async(_iopub_queue, ^{
        const int buffer_size = 1 << 21; // 2MB, might not be enough?
        char *buffer = malloc(buffer_size);

        while (_ioPort)
        {
            NSMutableArray *parts = [NSMutableArray array];
            int more = 0;
            do
            {
                char *msg = buffer;
                int msglen = buffer_size;
                more = s_recv_more(_ioPort, msg, &msglen);
                if (msg)
                {
                    id part = [self parseMessage:msg];
                    if (part == nil)
                    {
                        if (zmq_errno() == ETERM)
                        {
                            free(buffer), buffer = NULL;
                            zmq_close(_ioPort), _ioPort = NULL;
                            NSLog(@"io port terminated");
                            return;
                        }
                        printf("<< IOPub - could not parse message part: %s\n", msg);
                        continue;
                    }
                    [parts addObject:part];
                    if (_debugLoggingEnabled)
                    {
                        printf("<< IOPub: %s\n", msg);
                    }
                }
            } while (more);
            
            if (zmq_errno() == ETERM)
            {
                free(buffer), buffer = NULL;
                zmq_close(_ioPort), _ioPort = NULL;
                NSLog(@"io port terminated");
                return;
            }
            
            Message *response = [Message messageFromParts:parts];
            [self dispatchResponse:response port:_ioPort];
            if (response.type == MessageTypeKernelShutdownReply)
            {
                free(buffer), buffer = NULL;
                return;
            }
        }
        free(buffer), buffer = NULL;
    });
    
    // -- Shell --
    dispatch_sync(_shell_queue, ^{
        int shellPort = [[_configuration objectForKey:ShellPortKey] intValue];
        NSString *addr = [NSString stringWithFormat:@"tcp://%@:%d", ipAddr, shellPort];
        
        // create dealer socket
        _shellPort = zmq_socket(_zmqContext, ZMQ_DEALER);
        const char *identify = [ShellPortIdentify UTF8String];
        zmq_setsockopt(_shellPort, ZMQ_IDENTITY, identify, strlen(identify));
        // connect
        int rc = zmq_connect(_shellPort, [addr UTF8String]);
        assert(rc == 0);
    });
}

- (void)close
{
    NSLog(@"closing io port");
    zmq_close(_ioPort), _ioPort = NULL;

    NSLog(@"closing shell port");
    zmq_close(_shellPort), _shellPort = NULL;
}

#pragma mark - Messages

- (void)sendShellMessage:(Message *)message
{
    if (message == nil)
    {
        return;
    }
    [_requests addObject:message];
    [_pendingRequests addObject:message];
    
    dispatch_async(_shell_queue, ^{
        for (NSUInteger idx = 0; idx < [message.parts count] - 1; idx++)
        {
            char *part = (char *)[[message.parts objectAtIndex:idx] UTF8String];
            if (_debugLoggingEnabled)
            {
                printf(">> Shell: %s\n", part);
            }
            s_sendmore(_shellPort, part);
        }
        char *finalPart = (char *)[[message.parts lastObject] UTF8String];
        if (_debugLoggingEnabled)
        {
            printf(">> Shell: %s\n", finalPart);
        }
        s_send(_shellPort, finalPart);
        
        NSMutableArray *parts = [NSMutableArray array];
        int more = 0;
        
        const int buffer_size = 1 << 20; // 1MB, might not be enough?
        char *buffer = malloc(buffer_size);
        
        do
        {
            char *msg = buffer;
            int msglen = buffer_size;
            more = s_recv_more(_shellPort, msg, &msglen);
            if (msg)
            {
                id part = [self parseMessage:msg];
                if (part == nil)
                {
                    if (zmq_errno() == ETERM)
                    {
                        free(buffer), buffer = NULL;
                        zmq_close(_shellPort), _shellPort = NULL;
                        NSLog(@"shell port terminated");
                        return;
                    }
                    printf("<< Shell - could not parse message part: %s\n", msg);
                    continue;
                }
                [parts addObject:part];
                if (_debugLoggingEnabled)
                {
                    printf("<< Shell: %s\n", msg);
                }
            }
        } while (more);
        
        free(buffer), buffer = NULL;
        if (zmq_errno() == ETERM)
        {
            zmq_close(_shellPort), _shellPort = NULL;
            NSLog(@"shell port terminated");
            return;
        }
        
        Message *response = [Message messageFromParts:parts];
        [self dispatchResponse:response port:_shellPort];
    });
}

#pragma mark - Requests

- (void)sendKernelInfoRequest:(RequestBlock)completion
{
    NSLog(@"sending kernel info request...");
    Message *message = [Message messageWithIdentity:ShellPortIdentify
                                          sessionId:_sessionId
                                               type:MessageTypeKernelInfoRequest
                                         completion:completion];
    [self sendShellMessage:message];
}

- (void)sendKernelShutdownRequest:(RequestBlock)completion
{
    NSLog(@"sending kernel shutdown request...");
    Message *message = [Message messageWithIdentity:ShellPortIdentify
                                          sessionId:_sessionId
                                               type:MessageTypeKernelShutdownRequest
                                         completion:completion];
    [self sendShellMessage:message];
}

- (void)sendKernelRestartRequest:(RequestBlock)completion
{
    NSLog(@"sending kernel restart request...");
    Message *message = [Message messageWithIdentity:ShellPortIdentify
                                          sessionId:_sessionId
                                               type:MessageTypeKernelShutdownRequest
                                         completion:completion];
    message.content[ @"restart" ] = @(YES);
    [self sendShellMessage:message];
}

- (void)sendExecuteRequest:(NSString *)code completion:(RequestBlock)completion
{
    NSLog(@"sending execute request...");
    Message *message = [Message messageWithIdentity:ShellPortIdentify
                                          sessionId:_sessionId
                                               type:MessageTypeExecuteRequest
                                         completion:completion];
    message.content[ @"code" ] = code;
    message.content[ @"silent" ] = @(NO);
    message.content[ @"store_history" ] = @(YES);
    message.content[ @"user_variables" ] = @[ ];
    message.content[ @"user_expressions" ] = @{ };
    message.content[ @"allow_stdin" ] = @(NO);
    [self sendShellMessage:message];
}

#pragma mark - Responses

- (void)dispatchResponse:(Message *)response port:(void *)port
{
    if (response == nil)
    {
        return;
    }
    for (Message *request in _pendingRequests)
    {
        if ([response.parentMsgId isEqualToString:request.msgId])
        {
            if (port == _shellPort)
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if ([_delegate shellMessage:request receivedResponse:response])
                    {
                        [_pendingRequests removeObject:request];
                    }
                });
            }
            else if (port == _ioPort)
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    if ([_delegate iopubMessage:request receivedResponse:(Message *)response])
                    {
                        [_pendingRequests removeObject:request];
                    }
                });
            }
            break;
        }
    }
}

@end
