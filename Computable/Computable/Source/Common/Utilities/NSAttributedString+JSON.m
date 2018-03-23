//
//  NSAttributedString+JSON.m
//  Computable
//
//  Created by Karl Traunmüller on 08.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSAttributedString+JSON.h"
#import "NSString+JSON.h"

@implementation NSAttributedString (JSON)

- (NSMutableDictionary *)asJsonDictionary
{
    NSMutableDictionary *result = [self.string asJsonDictionary];
    return result;
}

@end
