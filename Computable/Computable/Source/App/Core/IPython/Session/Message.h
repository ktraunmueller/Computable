//
//  Message.h
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 05.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, MessageType)
{
    MessageTypeUnknown,
    // kernel management
    MessageTypeStatus,
    MessageTypeKernelInfoRequest,
    MessageTypeKernelInfoReply,
    MessageTypeKernelShutdownRequest,
    MessageTypeKernelShutdownReply,
    // execute
    MessageTypeExecuteRequest,
    MessageTypeExecuteReply,
    // i/o
    MessageTypePyIn,
    MessageTypePyOut,
    MessageTypePyErr,
    MessageTypeStream,
    MessageTypeDisplayData,
    MessageTypeDataPub,
};

typedef NS_ENUM(NSUInteger, StreamType)
{
    StreamTypeUnknown,
    StreamTypeStdOut,
    StreamTypeStdErr,
};

typedef NS_ENUM(NSUInteger, Status)
{
    StatusUnkown,
    StatusOk,
    StatusError,
    StatusAbort,
};

typedef NS_ENUM(NSUInteger, ExecutionState)
{
    ExecutionStateUnknown,
    ExecutionStateIdle,
    ExecutionStateBusy,
};

typedef NS_ENUM(NSUInteger, MimeType)
{
    MimeTypeUnknown,
    MimeTypeTextPlain,
    MimeTypeImagePng,
};

@class Message;

typedef void (^RequestBlock)(Message *request, Message *response);

extern NSDateFormatter *MessageDateFormatter;

@interface Message : NSObject

// create an outgoing message
+ (Message *)messageWithIdentity:(NSString *)identity
                       sessionId:(NSString *)sessionId
                            type:(MessageType)type
                      completion:(RequestBlock)completion;

// parse an incoming message
+ (Message *)messageFromParts:(NSArray *)parts;

@property (nonatomic, readonly) NSString *identity;
@property (nonatomic, readonly) NSString *sessionId;
@property (nonatomic, readonly) NSString *username;
@property (nonatomic, readonly) MessageType type;
@property (nonatomic, readonly) NSString *msgId;
@property (nonatomic, readonly) RequestBlock completion;
// response properties
@property (nonatomic, readonly) NSString *parentMsgId;
@property (nonatomic, readonly) NSDate *date;
@property (nonatomic, readonly) NSMutableDictionary *content;   // TODO why mutable?
// replies
@property (nonatomic, readonly) BOOL isExecuteReply;
@property (nonatomic, readonly) BOOL isStatus;
@property (nonatomic, readonly) Status status;
@property (nonatomic, readonly) ExecutionState executionState;
@property (nonatomic, readonly) NSUInteger executionCount;
// stream messages (e.g., print statements)
@property (nonatomic, readonly) BOOL isStream;
@property (nonatomic, readonly) StreamType streamType;
@property (nonatomic, readonly) NSString *streamName;
@property (nonatomic, readonly) NSString *streamData;
// input messages
@property (nonatomic, readonly) BOOL isInput;
@property (nonatomic, readonly) NSString *input;
// output messages
@property (nonatomic, readonly) BOOL isOutput;
@property (nonatomic, readonly) BOOL isDisplayData;
@property (nonatomic, readonly) NSString *output;
@property (nonatomic, readonly) NSArray *outputMimeTypes;
// error messages
@property (nonatomic, readonly) BOOL isError;
@property (nonatomic, readonly) NSString *errorValue;
@property (nonatomic, readonly) NSString *errorName;
@property (nonatomic, readonly) NSArray *errorTraceback;

@property (nonatomic, readonly) NSArray *parts;

+ (NSString *)typeString:(MessageType)type;

- (id)outputForMimeType:(MimeType)mimeType;
- (id)rawContentForMimeType:(MimeType)mimeType;

@end
