//
//  Migrations.m
//  Computable
//
//  Created by Karl Traunmüller on 04.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Migrations.h"
#import "../../../Common/Utilities/Filesystem.h"

static NSString *const kMigrationsFile = @"migrations.plist";
static NSString *const kLastMigrationVersion = @"LastMigrationVersion";
static NSString *const kCopyFilesKey = @"CopyFiles";
static NSString *const kApplicationSupportPrefix = @"ApplicationSupport";

static NSDictionary *_migrations;

@implementation Migrations

+ (void)readMigrationsFile
{
    if (_migrations)
    {
        return;
    }
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *migrationsFile = [bundle pathForResource:[kMigrationsFile stringByDeletingPathExtension]
                                                ofType:[kMigrationsFile pathExtension]];
    if (migrationsFile == nil)
    {
        return;
    }
    _migrations = [NSDictionary dictionaryWithContentsOfFile:migrationsFile];
}

+ (BOOL)hasPendingMigrations {
    [Migrations readMigrationsFile];
    if (_migrations == nil)
    {
        return NO;
    }
    
    NSArray *versions = [Migrations sortedMigrationVersions];
    float highestMigrationVersion = [versions.lastObject floatValue];
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    float lastMigrationVersion = [userDefaults floatForKey:kLastMigrationVersion];
    return highestMigrationVersion > lastMigrationVersion;
}

+ (NSArray *)sortedMigrationVersions
{
    return [[_migrations allKeys] sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        NSString *key1 = obj1;
        NSString *key2 = obj2;
        float version1 = [key1 floatValue];
        float version2 = [key2 floatValue];
        return version1 < version2 ? NSOrderedAscending : (version1 > version2 ? NSOrderedDescending : NSOrderedSame);
    }];
}

+ (void)runPendingMigrations:(dispatch_block_t)completion {
    if (_migrations == nil)
    {
        [Migrations readMigrationsFile];
    }
    if (_migrations == nil)
    {
        if (completion)
        {
            completion();
        }
        return;
    }
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
        float lastMigrationVersion = [userDefaults floatForKey:kLastMigrationVersion];
        
        NSBundle *bundle = [NSBundle mainBundle];
        NSString *applicationSupportDirectory = [NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES) firstObject];
        NSFileManager *fileManager = [NSFileManager defaultManager];
        for (NSString *version in [Migrations sortedMigrationVersions])
        {
            if ([version floatValue] < lastMigrationVersion)
            {
                continue;   // skip old migrations
            }
            NSDictionary *migrations = [_migrations objectForKey:version];
            NSDictionary *files = [migrations objectForKey:kCopyFilesKey];
            for (NSString *fromFile in [files allKeys])
            {
                NSString *toFile = [files objectForKey:fromFile];
                if ([toFile hasPrefix:kApplicationSupportPrefix] && toFile.length > kApplicationSupportPrefix.length)
                {
                    NSString *path = [toFile substringFromIndex:kApplicationSupportPrefix.length + 1];
                    NSArray *srcPathComponents = [fromFile pathComponents];
                    NSString *directory = [srcPathComponents count] > 1 ? [fromFile stringByDeletingLastPathComponent] : nil;
                    NSString *file = [fromFile lastPathComponent];
                    NSString *resource = [file stringByDeletingPathExtension];
                    NSString *type = [file pathExtension];
                    NSString *dstPath = [applicationSupportDirectory stringByAppendingPathComponent:path];
                    NSString *srcPath = [bundle pathForResource:resource ofType:type inDirectory:directory];
                    NSError *error = nil;
                    [fileManager copyItemAtPath:srcPath toPath:dstPath error:&error];
                    if (error)
                    {
                        NSLog(@"%@", error);
                    }
                    continue;
                }
            }
            [userDefaults setObject:version forKey:kLastMigrationVersion];
            [userDefaults synchronize];
        }
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion();
            });
        }
    });
}

@end
