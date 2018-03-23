//
//  NSString+JSON.m
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSString+JSON.h"

@implementation NSString (JSON)

- (NSMutableDictionary *)asJsonDictionary
{
    NSData *data = [self dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    id result = [NSJSONSerialization JSONObjectWithData:data
                                                options:NSJSONReadingMutableContainers
                                                  error:&error];
    if (error)
    {
        NSLog(@"%@", error);
    }
    if ([result isKindOfClass:[NSDictionary class]])
    {
        result = [NSMutableDictionary dictionaryWithDictionary:result];
    }
    else
    {
        result = nil;
    }
    return result;
}

@end
