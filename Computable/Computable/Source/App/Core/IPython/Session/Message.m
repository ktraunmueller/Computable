//
//  Message.m
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 05.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Message.h"
#import "../../../../Common/Utilities/NSDictionary+JSON.h"
#import "../../../../Common/Utilities/NSString+JSON.h"
#import "../../../../Common/Utilities/NSString+PathUtils.h"
#import "../../../../Common/Utilities/NSString+UUID.h"
#import "../../../../Common/Utilities/UIImage+Base64.h"

// wire protocol: http://ipython.org/ipython-doc/dev/development/messaging.html

static NSString *const kMsgId = @"msg_id";
static NSString *const kUsername = @"username";
static NSString *const kSession = @"session";
static NSString *const kMsgType = @"msg_type";
static NSString *const kDate = @"date";
static NSString *const kMsgDelim = @"<IDS|MSG>";

static NSString *const kStatus = @"status";
static NSString *const kOk = @"ok";
static NSString *const kAbort = @"abort";
static NSString *const kError = @"error";
static NSString *const kExecutionState = @"execution_state";
static NSString *const kExecutionStateIdle = @"idle";
static NSString *const kExecutionStateBusy = @"busy";
static NSString *const kExecutionCount = @"execution_count";
static NSString *const kCode = @"code";
static NSString *const kData = @"data";
static NSString *const kEValue = @"evalue";
static NSString *const kEName = @"ename";
static NSString *const kTraceback = @"traceback";

static NSString *const kPyIn = @"pyin";
static NSString *const kPyOut = @"pyout";
static NSString *const kPyErr = @"pyerr";
static NSString *const kStream = @"stream";
static NSString *const kStreamName = @"name";
static NSString *const kStreamStdOut = @"stdout";
static NSString *const kStreamStdErr = @"stderr";
static NSString *const kDisplayData = @"display_data";
static NSString *const kDataPub = @"data_pub";

static NSString *const kKernelInfoRequest = @"kernel_info_request";
static NSString *const kKernelInfoReply = @"kernel_info_reply";
static NSString *const kShutdownRequest = @"shutdown_request";
static NSString *const kShutdownReply = @"shutdown_reply";
static NSString *const kExecuteRequest = @"execute_request";
static NSString *const kExecuteReply = @"execute_reply";

static NSString *const kMimeTypeTextPlain = @"text/plain";
static NSString *const kMimeTypeImagePng = @"image/png";

NSDateFormatter *MessageDateFormatter;

@implementation Message
{
    NSMutableDictionary *_header;
    NSMutableDictionary *_parentHeader;
    NSMutableDictionary *_metadata;
    NSMutableArray *_parts;
}

#pragma mark - Statics

+ (void)initialize
{
    MessageDateFormatter = [[NSDateFormatter alloc] init];
    MessageDateFormatter.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss.SSSSSS";
    MessageDateFormatter.locale = [NSLocale localeWithLocaleIdentifier:@"en_US_POSIX"];
}

+ (Message *)messageWithIdentity:(NSString *)identity
                       sessionId:(NSString *)sessionId
                            type:(MessageType)type
                      completion:(RequestBlock)completion
{
    Message *message = [[Message alloc] initWithWithIdentity:identity
                                                   sessionId:sessionId
                                                        type:type
                                                  completion:completion];
    return message;
}

+ (Message *)messageFromParts:(NSArray *)parts
{
    if ([parts count] == 0)
    {
        return nil;
    }
    NSString *identity = nil;
    NSString *delim = nil;
    NSString *signature = nil;
    NSMutableDictionary *header = nil;
    NSMutableDictionary *parentHeader = nil;
    NSMutableDictionary *metadata = nil;
    NSMutableDictionary *content = nil;
    
    NSMutableArray *mutableParts = [NSMutableArray arrayWithArray:parts];
    if ([mutableParts count] > 0)
    {
        NSString *part = [mutableParts firstObject];
        if (![part isEqualToString:kMsgDelim])
        {
            identity = part;
            [mutableParts removeObjectAtIndex:0];
        }
    }
    if ([mutableParts count] > 0)
    {
        NSString *part = [mutableParts firstObject];
        if ([part isEqualToString:kMsgDelim])
        {
            delim = part;
            [mutableParts removeObjectAtIndex:0];
        }
        else
        {
            NSLog(@"expected message delimiter");
        }
    }
    if ([mutableParts count] > 0)
    {
        signature = [mutableParts firstObject];
        [mutableParts removeObjectAtIndex:0];
    }
    if ([mutableParts count] > 0)
    {
        NSString *json = [mutableParts firstObject];
        [mutableParts removeObjectAtIndex:0];
        header = [json asJsonDictionary];
    }
    if ([mutableParts count] > 0)
    {
        NSString *json = [mutableParts firstObject];
        [mutableParts removeObjectAtIndex:0];
        parentHeader = [json asJsonDictionary];
    }
    if ([mutableParts count] > 0)
    {
        NSString *json = [mutableParts firstObject];
        [mutableParts removeObjectAtIndex:0];
        metadata = [json asJsonDictionary];
    }
    if ([mutableParts count] > 0)
    {
        // json may be an NSString or NSAttributedString
        id json = [mutableParts firstObject];
        [mutableParts removeObjectAtIndex:0];
        content = [json asJsonDictionary];
    }
    // TODO handle data buffers
    return [[Message alloc] initWithIdentity:identity
                                      header:header
                                parentHeader:parentHeader
                                    metadata:metadata
                                     content:content];
}

+ (NSString *)typeString:(MessageType)type
{
    switch (type) {
        case MessageTypeStatus: return kStatus;
            // kernel info
        case MessageTypeKernelInfoRequest: return kKernelInfoRequest;
        case MessageTypeKernelInfoReply: return kKernelInfoReply;
            // kernel shutdown
        case MessageTypeKernelShutdownRequest: return kShutdownRequest;
        case MessageTypeKernelShutdownReply: return kShutdownReply;
            // execute
        case MessageTypeExecuteRequest: return kExecuteRequest;
        case MessageTypeExecuteReply: return kExecuteReply;
            // i/o
        case MessageTypePyIn: return kPyIn;
        case MessageTypePyOut: return kPyOut;
        case MessageTypePyErr: return kPyErr;
        case MessageTypeStream: return kStream;
        case MessageTypeDisplayData: return kDisplayData;
        case MessageTypeDataPub: return kDataPub;
        default:
            break;
    }
    return nil;
}

+ (MessageType)typeFromString:(NSString *)type
{
    if ([type isEqualToString:kStatus])
    {
        return MessageTypeStatus;
    }
    if ([type isEqualToString:kPyIn])
    {
        return MessageTypePyIn;
    }
    if ([type isEqualToString:kPyOut])
    {
        return MessageTypePyOut;
    }
    if ([type isEqualToString:kPyErr])
    {
        return MessageTypePyErr;
    }
    if ([type isEqualToString:kStream])
    {
        return MessageTypeStream;
    }
    if ([type isEqualToString:kDisplayData])
    {
        return MessageTypeDisplayData;
    }
    if ([type isEqualToString:kDataPub])
    {
        return MessageTypeDataPub;
    }
    if ([type isEqualToString:kKernelInfoRequest])
    {
        return MessageTypeKernelInfoRequest;
    }
    if ([type isEqualToString:kKernelInfoReply])
    {
        return MessageTypeKernelInfoReply;
    }
    if ([type isEqualToString:kShutdownReply])
    {
        return MessageTypeKernelShutdownReply;
    }
    if ([type isEqualToString:kExecuteRequest])
    {
        return MessageTypeExecuteRequest;
    }
    if ([type isEqualToString:kExecuteReply])
    {
        return MessageTypeExecuteReply;
    }
    // TODO clear_output
    NSLog(@"unknown message: %@", type);
    return MessageTypeUnknown;
}

+ (Status)statusFromString:(NSString *)status
{
    if ([status isEqualToString:kOk])
    {
        return StatusOk;
    }
    if ([status isEqualToString:kAbort])
    {
        return StatusAbort;
    }
    if ([status isEqualToString:kError])
    {
        return StatusError;
    }
    NSLog(@"unknown status: %@", status);
    return StatusUnkown;
}

+ (ExecutionState)executionStateFromString:(NSString *)state
{
    if ([state isEqualToString:kExecutionStateIdle])
    {
        return ExecutionStateIdle;
    }
    if ([state isEqualToString:kExecutionStateBusy])
    {
        return ExecutionStateBusy;
    }
    NSLog(@"unknown execution state: %@", state);
    return ExecutionStateUnknown;
}

+ (MimeType)mimeTypeFromString:(NSString *)mimeType
{
    if ([mimeType isEqualToString:kMimeTypeTextPlain])
    {
        return MimeTypeTextPlain;
    }
    if ([mimeType isEqualToString:kMimeTypeImagePng])
    {
        return MimeTypeImagePng;
    }
    return MimeTypeUnknown;
}

+ (NSString *)mimeTypeString:(MimeType)mimeType
{
    switch (mimeType) {
        case MimeTypeTextPlain: return kMimeTypeTextPlain;
        case MimeTypeImagePng: return kMimeTypeImagePng;
        default:
            break;
    }
    NSLog(@"unhandled mime type");
    return nil;
}

#pragma mark - Message

- (id)initWithWithIdentity:(NSString *)identify
                 sessionId:(NSString *)sessionId
                      type:(MessageType)type
                completion:(RequestBlock)completion
{
    if (self = [super init])
    {
        _identity = identify;
        _sessionId = sessionId;
        _type = type;
        _msgId = [[NSString stringWithUUID] lowercaseString];
        _username = @"Computable";
        _completion = completion;
        
        _header = [ @{ kMsgId : _msgId,
                       kUsername : _username,
                       kSession : _sessionId,
                       kMsgType : [Message typeString:_type]
                       } mutableCopy ];
        _parentHeader = [NSMutableDictionary dictionary];
        _metadata = [NSMutableDictionary dictionary];
        _content = [NSMutableDictionary dictionary];
    }
    return self;
}

- (id)initWithIdentity:(NSString *)identity
                header:(NSMutableDictionary *)header
          parentHeader:(NSMutableDictionary *)parentHeader
              metadata:(NSMutableDictionary *)metadata
               content:(NSMutableDictionary *)content
{
    if (self = [super init])
    {
        _identity = identity;
        _header = header ? header : [NSMutableDictionary dictionary];
        _parentHeader = parentHeader ? parentHeader : [NSMutableDictionary dictionary];
        _metadata = metadata ? metadata : [NSMutableDictionary dictionary];
        _content = content ? content : [NSMutableDictionary dictionary];
        
        if ([header objectForKey:kSession])
        {
            _sessionId = [header objectForKey:kSession];
        }
        if ([header objectForKey:kMsgId])
        {
            _msgId = [header objectForKey:kMsgId];
        }
        if ([header objectForKey:kUsername])
        {
            _username = [header objectForKey:kUsername];
        }
        if ([header objectForKey:kMsgType])
        {
            _type = [Message typeFromString:[header objectForKey:kMsgType]];
        }
        if ([header objectForKey:kDate])
        {
            _date = [MessageDateFormatter dateFromString:[header objectForKey:kDate]];
        }
    }
    return self;
}

- (NSString *)parentMsgId
{
    return [_parentHeader objectForKey:kMsgId];
}

- (NSArray *)parts
{
    if (_parts == nil)
    {
        _parts = [NSMutableArray array];
        [_parts addObject:_identity];
        [_parts addObject:kMsgDelim];
        [_parts addObject:@""]; // signature
        [_parts addObject:[_header asJsonString]];
        [_parts addObject:[_parentHeader asJsonString]];
        [_parts addObject:[_metadata asJsonString]];
        [_parts addObject:[_content asJsonString]];
    }
    return _parts;
}

- (BOOL)isExecuteReply
{
    return _type == MessageTypeExecuteReply;
}

- (BOOL)isStatus
{
    return _type == MessageTypeStatus;
}

- (Status)status
{
    return [Message statusFromString:_content[ kStatus ]];
}

- (ExecutionState)executionState
{
    return [Message executionStateFromString:_content [ kExecutionState ]];
}

- (NSUInteger)executionCount
{
    id count = [_content objectForKey:kExecutionCount];
    return count ? [count unsignedIntegerValue] : 0;
}

- (BOOL)isStream
{
    return _type == MessageTypeStream;
}

- (StreamType)streamType
{
    if (self.type != MessageTypeStream)
    {
        return StreamTypeUnknown;
    }
    NSString *stream = [_content objectForKey:kStreamName];
    if ([stream isEqualToString:kStreamStdOut])
    {
        return StreamTypeStdOut;
    }
    if ([stream isEqualToString:kStreamStdErr])
    {
        return StreamTypeStdErr;
    }
    NSLog(@"unkown stream type");
    return StreamTypeUnknown;
}

- (NSString *)streamName
{
    if (self.type != MessageTypeStream)
    {
        return nil;
    }
    return [_content objectForKey:kStreamName];
}

- (NSString *)streamData
{
    if (self.type != MessageTypeStream)
    {
        return nil;
    }
    return _content[ kData ];
}

- (BOOL)isInput
{
    return _type == MessageTypePyIn;
}

- (NSString *)input
{
    return self.isInput ? _content[ kCode ] : nil;
}

- (BOOL)isOutput
{
    return _type == MessageTypePyOut;
}

- (BOOL)isDisplayData
{
    return _type == MessageTypeDisplayData;
}

- (NSArray *)outputMimeTypes
{
    if (!self.isOutput)
    {
        return nil;
    }
    NSDictionary *data = _content[ kData ];
    return data.allKeys;
}

- (id)outputForMimeType:(MimeType)mimeType
{
    if (!(self.isOutput || self.isDisplayData))
    {
        return nil;
    }
    NSDictionary *data = _content[ kData ];
    id result = [data objectForKey:[Message mimeTypeString:mimeType]];
    switch (mimeType)
    {
        case MimeTypeTextPlain: return result;
        case MimeTypeImagePng: return [UIImage imageFromBase64String:result];
        default:
            break;
    }
    return nil;
}

- (id)rawContentForMimeType:(MimeType)mimeType
{
    if (!(self.isOutput || self.isDisplayData))
    {
        return nil;
    }
    NSDictionary *data = _content[ kData ];
    return [data objectForKey:[Message mimeTypeString:mimeType]];
}

- (BOOL)isError
{
    return _type == MessageTypePyErr;
}

- (NSString *)errorValue
{
    return self.isError ? [_content[ kEValue ] stringbyAbbreviatingWithTildeInPaths] : nil;
}

- (NSString *)errorName
{
    return self.isError ? _content[ kEName ] : nil;
}

- (NSArray *)errorTraceback
{
    if (!self.isError)
    {
        return nil;
    }
    // looks like this can also be a single string: https://rink.hockeyapp.net/manage/apps/61508/app_versions/42/crash_reasons/16026617
    id traceback = _content[ kTraceback ];
    if (traceback == nil)
    {
        return nil;
    }
    if ([traceback isKindOfClass:[NSArray class]])
    {
        return traceback;
    }
    return @[ traceback ];
}

#pragma mark - NSObject

- (NSString *)description
{
    return [self.parts componentsJoinedByString:@"\n"];
}

@end
