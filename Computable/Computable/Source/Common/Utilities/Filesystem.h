//
//  Filesystem.h
//  Computable
//
//  Created by Karl Traunmüller on 28.07.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Filesystem : NSObject

+ (NSString *)scopedApplicationSupportDirectory;

+ (BOOL)addSkipBackupAttributeToItemAtPath:(NSString *)path;

@end
