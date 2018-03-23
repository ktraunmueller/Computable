//
//  DropboxSync.h
//  Computable
//
//  Created by Karl Traunmüller on 11.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const DropboxWriteNotification;
extern NSString *const DropboxDownloadDidStartNotification;
extern NSString *const DropboxDownloadDidStopNotification;

@interface DropboxSync : NSObject

+ (DropboxSync *)instance;

+ (void)start;
+ (void)stop;

- (void)cacheAndRegisterFile:(NSString *)dropboxPath completion:(void(^)(NSString *localPath))completion;

@end
