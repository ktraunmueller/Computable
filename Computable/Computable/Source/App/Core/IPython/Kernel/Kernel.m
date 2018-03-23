//
//  Kernel.m
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Kernel.h"
#import "../IPythonApplication.h"
#import "../AnsiParser.h"
#import "../Session/Session.h"
#import "../../Python/Runtime/NSString+PythonRuntime.h"

@interface Kernel () <SessionDelegate>
@end

@implementation Kernel
{
    Session *_session;
}

#pragma mark - Statics

+ (Kernel *)newInstanceWithId:(NSString *)kernelId session:(Session *)session
{
    return [[Kernel alloc] initWithId:kernelId session:session];
}

#pragma mark - Kernel

- (id)initWithId:(NSString *)kernelId session:(Session *)session
{
    NSLog(@"+++ Kernel init +++");
    
    if (self = [super init])
    {
        _kernelId = kernelId;
        _session = session;
        _session.delegate = self;
    }
    return self;
}

- (void)dealloc
{
    NSLog(@"--- Kernel dealloc ---");
}

#pragma mark - Kernel Management

- (void)stop:(dispatch_block_t)completion
{
    [_session sendKernelShutdownRequest:^(Message *request, Message *response) {
    }];
    
    double delayInSeconds = 0.2;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [IPythonApplication doOneIteration:self];
        
        NSLog(@"IPython kernel stopped");
        if (completion)
        {
            completion();
        }
    });
}

- (void)interrupt
{
    // TODO clear pending requests?
    [IPythonApplication interruptKernel:self];
}

#pragma mark - Execution

- (void)execute:(NSString *)code completion:(RequestBlock)completion
{
    [_session sendExecuteRequest:code completion:completion];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [IPythonApplication doOneIteration:self];
    });
}

#pragma mark - Response Handler

- (void)handleKernelInfoReply:(Message *)response request:(Message *)request
{
    NSArray *versionInfo = response.content[ @"ipython_version" ];
    NSLog(@"IPython version: %@", [versionInfo componentsJoinedByString:@"."]);
    [self handleGenericReply:response request:request];
}

- (void)handleKernelShutdownReply:(Message *)response request:(Message *)request
{
    [_session close], _session = nil;
    [self handleGenericReply:response request:request];
}

- (void)handleGenericReply:(Message *)response request:(Message *)request
{
    if (response.type == MessageTypeStatus)
    {
        NSLog(@"kernel status: %@", response.content);
    }
    else if (response.type == MessageTypePyErr)
    {
        [self logErrorTraceback:response];
    }
    if (request.completion)
    {
        request.completion(request, response);
    }
}

- (void)handleDataReply:(Message *)response request:(Message *)request
{
    if (request.completion)
    {
        request.completion(request, response);
    }
}

- (void)logErrorTraceback:(Message *)response
{
    // TODO show traceback in UI somewhere? (only logging to console is kind of pointless)
    if (response == nil || response.errorTraceback == nil)
    {
        return;
    }
    AnsiParser *parse = [AnsiParser new];
    for (NSString *entry in response.errorTraceback)
    {
        NSAttributedString *string = [parse parse:[entry UTF8String]];
        NSLog(@"%@", [string.string stringByStrippingPythonPath]);
    }
}

#pragma mark - SessionDelegate

- (BOOL)shellMessage:(Message *)message receivedResponse:(Message *)response
{
    if (response == nil)
    {
        NSLog(@"response is nil");
        return YES;
    }
    NSLog(@"shell message received: %@", [Message typeString:response.type]);
    
    switch (response.type) {
        case MessageTypeKernelInfoReply:
            [self handleKernelInfoReply:response request:message];
            return YES;
            break;
        case MessageTypeKernelShutdownReply:
            [self handleKernelShutdownReply:response request:message];
            return YES;
        case MessageTypeExecuteReply:
            [self handleGenericReply:response request:message];
            return YES;
        default:
            break;
    }
    NSLog(@"unhandled shell reply");
    return YES; // ignore
}

- (BOOL)iopubMessage:(Message *)message receivedResponse:(Message *)response
{
    if (response == nil)
    {
        NSLog(@"response is nil");
        return YES;
    }
    NSLog(@"iopub message received: %@", [Message typeString:response.type]);
    
    switch (response.type) {
        case MessageTypeStatus:
            [self handleGenericReply:response request:message];
            return NO;
        case MessageTypeKernelShutdownReply:
            [self handleKernelShutdownReply:response request:message];
            return YES;
        case MessageTypeExecuteReply:
        case MessageTypePyErr:
            [self handleGenericReply:response request:message];
            return YES;
        case MessageTypeDisplayData:
        case MessageTypeDataPub:
            [self handleDataReply:response request:message];
            return NO;
        case MessageTypePyIn:
            [self handleGenericReply:response request:message];
            return NO; // re-broadcast of the execute_request, continue request processing
        case MessageTypePyOut:
            [self handleGenericReply:response request:message];
            return NO;
        case MessageTypeStream:
            [self handleGenericReply:response request:message];
            return NO;
        default:
            break;
    }
    NSLog(@"unhandled iopub reply");
    return YES; // ignore
}

#pragma mark - Utilities

- (NSDictionary *)connectionConfigurationFromFile:(NSString *)connectionConfigurationPath
{
    NSData *configurationData = [NSData dataWithContentsOfFile:connectionConfigurationPath];
    NSError *error = nil;
    NSDictionary *configurationDict = [NSJSONSerialization JSONObjectWithData:configurationData options:0 error:&error];
    if (error)
    {
        NSLog(@"%@", error);
    }
    return configurationDict;
}

@end
