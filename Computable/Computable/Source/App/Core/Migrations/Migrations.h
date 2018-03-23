//
//  Migrations.h
//  Computable
//
//  Created by Karl Traunmüller on 04.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Migrations : NSObject

+ (BOOL)hasPendingMigrations;
+ (void)runPendingMigrations:(dispatch_block_t)completion;

@end
