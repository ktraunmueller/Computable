//
//  Dropbox.m
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Dropbox.h"

#import <Dropbox/Dropbox.h>

NSString *const DropboxFolder = @"Dropbox";
NSString *kDropboxUrlScheme;

static NSString *const kDropboxAppKey = @"--REMOVED--";
static NSString *const kDropboxAppSecret = @"--REMOVED--";

@implementation Dropbox

#pragma mark - Statics

+ (void)initialize
{
    kDropboxUrlScheme = [NSString stringWithFormat:@"db-%@", kDropboxAppKey];
}

+ (NSString *)userHomeRelativePath:(NSString *)dropboxPath
{
    if (dropboxPath == nil)
    {
        return nil;
    }
    NSString *documentsFolder = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, NO) firstObject];
    return [[documentsFolder stringByAppendingPathComponent:DropboxFolder] stringByAppendingPathComponent:dropboxPath];
}

+ (void)setup
{
    DBAccountManager *accountManager = [[DBAccountManager alloc] initWithAppKey:kDropboxAppKey secret:kDropboxAppSecret];
	[DBAccountManager setSharedManager:accountManager];
    if (accountManager.linkedAccount != nil)
    {
        DBFilesystem *filesystem = [[DBFilesystem alloc] initWithAccount:accountManager.linkedAccount];
        [DBFilesystem setSharedFilesystem:filesystem];
    }
}

+ (BOOL)isLinked
{
    DBAccountManager *accountManager = [DBAccountManager sharedManager];
    DBAccount *account = accountManager.linkedAccount;
    return account != nil;
}

+ (NSString *)username
{
    DBAccountManager *accountManager = [DBAccountManager sharedManager];
    DBAccount *dropboxAccount = accountManager.linkedAccount;
    return dropboxAccount != nil ? dropboxAccount.info.userName : nil;
}

+ (BOOL)openURL:(NSURL *)url
{
    if (url == nil)
    {
        return NO;
    }
    DBAccount *account = [[DBAccountManager sharedManager] handleOpenURL:url];
    if (account)
    {
        DBFilesystem *filesystem = [[DBFilesystem alloc] initWithAccount:account];
        [DBFilesystem setSharedFilesystem:filesystem];
        return YES;
    }
    return NO;
}

+ (void)listFolderContents:(DBPath *)path completion:(void (^)(NSArray *pathContents))completion
{
    if (path == nil)
    {
        return;
    }
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        NSError *error = nil;
        DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
        NSArray *pathContents = [fileSystem listFolder:path error:&error];
        if (error != nil)
        {
            NSLog(@"%@", error);
        }
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(pathContents);
            });
        }
    });
}

+ (void)collectFolderContentsRecursively:(DBPath *)path into:(NSMutableArray *)contents
{
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    NSError *error = nil;
    NSArray *pathContents = [fileSystem listFolder:path error:&error];
    if (error != nil)
    {
        NSLog(@"%@", error);
        return;
    }
    for (DBFileInfo *fileInfo in pathContents)
    {
        // TODO collect folders also (e.g., sync folder deletions)
        if (fileInfo.isFolder)
        {
            [Dropbox collectFolderContentsRecursively:fileInfo.path into:contents];
        }
        else
        {
            [contents addObject:fileInfo];
        }
    }
}

+ (void)listFolderContentsRecursively:(DBPath *)path completion:(void (^)(NSArray *pathContents))completion
{
    if (path == nil)
    {
        return;
    }
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        NSMutableArray *result = [NSMutableArray array];
        [Dropbox collectFolderContentsRecursively:path into:result];
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(result);
            });
        }
    });
}

+ (void)writeFile:(NSString *)fileSystemPath toPath:(NSString *)dropboxPath error:(DBError **)error
{
    if (fileSystemPath == nil || dropboxPath == nil)
    {
        return;
    }
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    if (fileSystem == nil)
    {
        return;
    }
    
    DBError *dbError = error ? *error : nil;
    DBPath *path = [[DBPath root] childPath:dropboxPath];
    DBFileInfo *fileInfo = [fileSystem fileInfoForPath:path error:&dbError];
    DBFile *file = nil;
    if (fileInfo == nil)
    {
        file = [fileSystem createFile:path error:&dbError];
    }
    else
    {
        file = [fileSystem openFile:path error:&dbError];
    }
    if (file == nil)
    {
        NSLog(@"error creating file in Dropbox: %@", dropboxPath);
        return;
    }
    NSData *data = [NSData dataWithContentsOfFile:fileSystemPath];
    if (![file writeData:data error:&dbError])
    {
        NSLog(@"could not write data to file %@", dropboxPath);
    }
    [file close];
}

+ (void)copyFile:(NSString *)dropboxPath
{
    if (dropboxPath == nil)
    {
        return;
    }
    [self copyFile:dropboxPath toPath:[Dropbox fileSystemPath:dropboxPath]];
}

+ (void)copyFile:(NSString *)dropboxPath toPath:(NSString *)fileSystemPath
{
    if (dropboxPath == nil || fileSystemPath == nil)
    {
        return;
    }
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    if (fileSystem == nil)
    {
        return;
    }
    DBPath *path = [[DBPath root] childPath:dropboxPath];
    NSError *error = nil;
    DBFile *file = [fileSystem openFile:path error:&error];
    if (file == nil || error != nil)
    {
        NSLog(@"error opening file: %@", error != nil ? error : dropboxPath);
        return;
    }
    NSData *data = [file readData:&error];
    [file close];
    if (data == nil || error != nil)
    {
        NSLog(@"error reading file: %@", error != nil ? error : dropboxPath);
        return;
    }
    [[NSFileManager defaultManager] createDirectoryAtPath:[fileSystemPath stringByDeletingLastPathComponent]
                              withIntermediateDirectories:YES
                                               attributes:nil
                                                    error:NULL];
    [data writeToFile:fileSystemPath atomically:NO];
}

+ (void)deleteFile:(NSString *)dropboxPath
{
    if (dropboxPath == nil)
    {
        return;
    }
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    if (fileSystem == nil)
    {
        return;
    }
    
    DBError *error = nil;
    DBPath *path = [[DBPath root] childPath:dropboxPath];
    [fileSystem deletePath:path error:&error];
    if (error)
    {
        NSLog(@"error deleting file: %@", error);
    }
}

+ (BOOL)fileExistsLocally:(NSString *)dropboxPath
{
    if (dropboxPath == nil)
    {
        return NO;
    }
    NSString *localPath = [Dropbox fileSystemPath:dropboxPath];
    return [[NSFileManager defaultManager] fileExistsAtPath:localPath];
}

+ (NSString *)fileSystemPath:(NSString *)dropboxPath
{
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    return [[documentsDirectory stringByAppendingPathComponent:DropboxFolder] stringByAppendingPathComponent:dropboxPath];
}

@end
