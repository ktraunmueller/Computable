//
//  Session.h
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 05.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Message.h"

extern NSString *const ControlPortKey;
extern NSString *const HeartbeatPortKey;
extern NSString *const IOPubPortKey;
extern NSString *const ShellPortKey;
extern NSString *const StdinPortKey;
extern NSString *const IPAddressKey;
extern NSString *const DigestKey;

@protocol SessionDelegate;

@interface Session : NSObject

- (id)initWithId:(NSString *)sessionId
   configuration:(NSDictionary *)configuration;

@property (nonatomic) BOOL debugLoggingEnabled;
@property (nonatomic, readonly) NSString *sessionId;
@property (nonatomic, readonly) NSDictionary *configuration;
@property (nonatomic, weak) id<SessionDelegate> delegate;

- (void)start;
- (void)close;

- (void)sendKernelInfoRequest:(RequestBlock)completion;
- (void)sendKernelShutdownRequest:(RequestBlock)completion;
- (void)sendKernelRestartRequest:(RequestBlock)completion;
- (void)sendExecuteRequest:(NSString *)code completion:(RequestBlock)completion;

@end

@protocol SessionDelegate

- (BOOL)shellMessage:(Message *)message receivedResponse:(Message *)response;
- (BOOL)iopubMessage:(Message *)message receivedResponse:(Message *)response;

@end
