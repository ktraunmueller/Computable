//
//  Filesystem.m
//  Computable
//
//  Created by Karl Traunmüller on 28.07.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Filesystem.h"

@implementation Filesystem

+ (NSString *)scopedApplicationSupportDirectory
{
    NSString *applicationSupportDirectory = [NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES) firstObject];
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    return [applicationSupportDirectory stringByAppendingPathComponent:bundleIdentifier];
}

+ (BOOL)addSkipBackupAttributeToItemAtPath:(NSString *)path
{
    if (path == nil)
    {
        return NO;
    }
    NSURL *url = [NSURL fileURLWithPath:path];
    NSError *error = nil;
    BOOL success = [url setResourceValue:@YES
                                  forKey:NSURLIsExcludedFromBackupKey
                                   error:&error];
    if (!success)
    {
        NSLog(@"Error excluding %@ from iCloud backup %@", [url lastPathComponent], error);
    }
    else
    {
        NSLog(@"Excluded %@ from iCloud backup", path);
    }
    return success;
}

@end
