//
//  NSString+PathUtils.m
//  Computable
//
//  Created by Karl Traunmüller on 11.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSString+PathUtils.h"

@implementation NSString (PathUtils)

- (NSString *)stringbyAbbreviatingWithTildeInPaths
{
    NSString *homeDirectory = NSHomeDirectory();
    return [self stringByReplacingOccurrencesOfString:homeDirectory withString:@"~"];
}

@end
