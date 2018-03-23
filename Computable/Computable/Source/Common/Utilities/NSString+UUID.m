//
//  NSString+UUID.m
//  Taskdeck
//
//  Created by Karl Traunmüller on 16.04.13.
//  Copyright (c) 2013 Karl Traunmüller. All rights reserved.
//

#import "NSString+UUID.h"

@implementation NSString (UUID)

+ (NSString *)stringWithUUID
{
    CFUUIDRef uuidRef = CFUUIDCreate(nil);
    NSString *uuid = (NSString *)CFBridgingRelease(CFUUIDCreateString(nil, uuidRef));
    CFRelease(uuidRef);
    return uuid;
}

@end
