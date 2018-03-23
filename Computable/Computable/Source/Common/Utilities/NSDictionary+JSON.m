//
//  NSDictionary+JSON.m
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 04.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSDictionary+JSON.h"

@implementation NSDictionary (JSON)

- (NSString *)asJsonString
{
    NSError *error = nil;
    NSData *data = [NSJSONSerialization dataWithJSONObject:self
                                                   options:0
                                                     error:&error];
    if (error)
    {
        NSLog(@"%@", error);
    }
    return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}

- (void)writeToFile:(NSString *)path
{
    NSError *error = nil;
    NSData *data = [NSJSONSerialization dataWithJSONObject:self
                                                   options:NSJSONWritingPrettyPrinted
                                                     error:&error];
    if (error)
    {
        NSLog(@"error writing json to file: %@", error);
    }
    [data writeToFile:path atomically:YES];
}

@end
