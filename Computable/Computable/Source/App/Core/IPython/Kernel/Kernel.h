//
//  Kernel.h
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "../Session/Message.h"

@class Session;

@interface Kernel : NSObject

@property (nonatomic, readonly) NSString *kernelId;

+ (Kernel *)newInstanceWithId:(NSString *)kernelId session:(Session *)session;

- (void)stop:(dispatch_block_t)completion;
- (void)interrupt;

- (void)execute:(NSString *)code completion:(RequestBlock)completion;

@end