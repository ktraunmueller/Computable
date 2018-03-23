//
//  DropboxSync.m
//  Computable
//
//  Created by Karl Traunmüller on 11.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "DropboxSync.h"
#import "Dropbox.h"
#import "../../../Common/ThirdParty/DirectoryWatcher/DTFolderMonitor.h"
#import "../../../Common/ThirdParty/DirectoryWatcher/TABFileMonitor.h"

#import <Dropbox/Dropbox.h>

NSString *const DropboxWriteNotification = @"DropboxWriteNotification";
NSString *const DropboxDownloadDidStartNotification = @"DropboxDownloadDidStartNotification";
NSString *const DropboxDownloadDidStopNotification = @"DropboxDownloadDidStopNotification";

static DropboxSync *_dropboxSync;
static NSArray *_suppressedExtensions;

@interface DropboxSync () <TABFileMonitorDelegate>
@end

@implementation DropboxSync
{
    dispatch_queue_t _queue;
    NSString *_path;
    NSMutableArray *_downloadingFiles;
    NSMutableArray *_incomingFiles;
    NSMutableDictionary *_fileModificationDates;
    NSMutableArray *_directoryWatchers;
    NSMutableArray *_fileWatchers;
}

#pragma mark - Statics

+ (DropboxSync *)instance
{
    return _dropboxSync;
}

+ (void)start
{
    if (_dropboxSync)
    {
        return;
    }
    _suppressedExtensions = @[ @"py", @"ipynb", @".DS_Store" ];
    
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    NSString *dropboxPath = [documentsDirectory stringByAppendingPathComponent:DropboxFolder];
    _dropboxSync = [[DropboxSync alloc] initWithPath:dropboxPath];
    [_dropboxSync start];
}

+ (void)stop
{
    [_dropboxSync stop], _dropboxSync = nil;
}

#pragma mark - DropboxSync

- (id)initWithPath:(NSString *)path
{
    if (self = [super init])
    {
        _path = path;
        _queue = dispatch_queue_create(NULL, DISPATCH_QUEUE_SERIAL);
        _downloadingFiles = [NSMutableArray array];
        _incomingFiles = [NSMutableArray array];
    }
    return self;
}

- (void)start
{
    dispatch_async(_queue, ^{
        BOOL isDirectory = NO;
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:_path isDirectory:&isDirectory])
        {
            [fileManager createDirectoryAtPath:_path
                   withIntermediateDirectories:YES
                                    attributes:nil
                                         error:NULL];
        }
        
        _directoryWatchers = [NSMutableArray array];
        _fileWatchers = [NSMutableArray array];
        [self scanPath];        
        [self startWatchingFolder:_path];
        
        DBPath *dropboxRoot = [DBPath root];
        DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
        [fileSystem addObserver:self forPathAndDescendants:dropboxRoot block:^{
            [self listDropboxFolderContents];
        }];
        [self listDropboxFolderContents];
    });
}

- (void)stop
{
    dispatch_async(_queue, ^{
        DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
        [fileSystem removeObserver:self];
        
        [self stopDirectoryWatchers];
        _fileModificationDates = nil;
    });
}

- (void)startWatchingFolder:(NSString *)path
{
    if (path == nil)
    {
        return;
    }
    for (DTFolderMonitor *directoryWatcher in _directoryWatchers)
    {
        if ([directoryWatcher.path isEqualToString:path])
        {
            return;
        }
    }
    BOOL isDirectory = NO;
    if (![[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory] || !isDirectory)
    {
        return;
    }
    
    NSLog(@"** Watching folder %@...", path);
    DTFolderMonitor *directoryWatcher = [DTFolderMonitor folderMonitorForURL:[NSURL fileURLWithPath:path] block:^{
        [self scanPath];
    }];
    [_directoryWatchers addObject:directoryWatcher];
    [directoryWatcher startMonitoring];
}

- (void)stopWatchingFolder:(NSString *)path
{
    if (path == nil)
    {
        return;
    }
    DTFolderMonitor *directoryWatcherToRemove = nil;
    for (DTFolderMonitor *directoryWatcher in _directoryWatchers)
    {
        if ([directoryWatcher.path isEqualToString:path])
        {
            [directoryWatcher stopMonitoring];
            directoryWatcherToRemove = directoryWatcher;
            break;
        }
    }
    if (directoryWatcherToRemove)
    {
        [_directoryWatchers removeObject:directoryWatcherToRemove];
    }
}

- (void)stopDirectoryWatchers
{
    for (DTFolderMonitor *directoryWatcher in _directoryWatchers)
    {
        NSLog(@"** Stop watching folder %@", directoryWatcher.path);
        [directoryWatcher stopMonitoring];
    }
    _directoryWatchers = [NSMutableArray array];
}

- (void)startWatchingFiles
{
    for (NSString *path in _fileModificationDates.allKeys)
    {
        [self startWatchingFile:path];
    }
}

- (void)startWatchingFile:(NSString *)path
{
    if (path == nil)
    {
        return;
    }
    for (TABFileMonitor *fileWatcher in _fileWatchers)
    {
        if ([fileWatcher.path isEqualToString:path])
        {
            return;
        }
    }
    BOOL isDirectory = NO;
    if (![[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory] || isDirectory)
    {
        return;
    }
    
    NSLog(@"** Watching file %@...", path);
    TABFileMonitor *fileWatcher = [[TABFileMonitor alloc] initWithPath:path delegate:self];
    [_fileWatchers addObject:fileWatcher];
}

- (void)stopWatchingFile:(NSString *)path
{
    TABFileMonitor *fileWatcherToStop = nil;
    for (TABFileMonitor *fileWatcher in _fileWatchers)
    {
        if ([fileWatcher.path isEqualToString:path])
        {
            fileWatcherToStop = fileWatcher;
            break;
        }
    }
    if (fileWatcherToStop)
    {
        NSLog(@"** Stop watching file %@", path);
        [_fileWatchers removeObject:fileWatcherToStop];
    }
}

- (void)stopFileWatchers
{
    if ([_fileWatchers count] > 0)
    {
        NSLog(@"** Stopping file watchers");
        _fileWatchers = [NSMutableArray array];
    }
}

- (void)scanPath
{
    [self stopFileWatchers];
    
    NSLog(@"** Scanning %@ for local changes...", _path);
    
    NSArray *previousFiles = [_fileModificationDates.allKeys copy];
    NSDictionary *previousFileModificationDates = [_fileModificationDates copy];
    
    _fileModificationDates = [NSMutableDictionary dictionary];
    NSFileManager *fileManager = [NSFileManager new];
    NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:_path];
    NSString *file = nil;
    while (file = [enumerator nextObject])
    {
        NSString *localPath = [_path stringByAppendingPathComponent:file];
        BOOL isDirectory = NO;
        if ([fileManager fileExistsAtPath:localPath isDirectory:&isDirectory] && !isDirectory)
        {
            NSDictionary *attributes = [fileManager attributesOfItemAtPath:localPath error:NULL];
            if (attributes)
            {
                [_fileModificationDates setObject:attributes.fileModificationDate forKey:localPath];
                [self startWatchingFile:localPath];
            }
        }
        if (isDirectory)
        {
            [self startWatchingFolder:localPath];
        }
    }
    
    if ([previousFiles count] == 0)
    {
        return; // first scan
    }
    
    // don't sync local deletions for now
    //    NSMutableArray *deletedFiles = [NSMutableArray array];
    //    for (NSString *file in previousFiles)
    //    {
    //        if (![_fileModificationDates.allKeys containsObject:file])
    //        {
    //            [deletedFiles addObject:file];
    //        }
    //    }
    //    if ([deletedFiles count] > 0)
    //    {
    //        NSLog(@"-- deleted local files: %@", deletedFiles);
    //        for (NSString *file in deletedFiles)
    //        {
    //            NSString *dropboxPath = [file substringFromIndex:_path.length];
    //            [Dropbox deleteFile:dropboxPath];
    //            [_fileModificationDates removeObjectForKey:file];
    //        }
    //    }
    
    NSMutableArray *newFiles = [NSMutableArray array];
    NSMutableArray *incomingNewFiles = [NSMutableArray array];
    NSArray *files = [_fileModificationDates.allKeys copy];
    for (NSString *file in files)
    {
        NSDate *previousFileModificationDate = [previousFileModificationDates objectForKey:file];
        if (previousFileModificationDate == nil)
        {
            if (![_incomingFiles containsObject:file])
            {
                [newFiles addObject:file];
            }
            else
            {
                [incomingNewFiles addObject:file];
            }
        }
    }
    [_incomingFiles removeObjectsInArray:incomingNewFiles];
    if ([newFiles count] > 0)
    {
        NSLog(@"++ new local files: %@", newFiles);
        for (NSString *file in newFiles)
        {
            NSString *dropboxPath = [file substringFromIndex:_path.length];
            [Dropbox writeFile:file toPath:dropboxPath error:NULL];
            dispatch_async(dispatch_get_main_queue(), ^{
                [[NSNotificationCenter defaultCenter] postNotificationName:DropboxWriteNotification object:file];
            });
            [self startWatchingFile:file];
        }
    }
}

- (void)listDropboxFolderContents
{
    dispatch_async(_queue, ^{
        DBPath *dropboxRoot = [DBPath root];
        [Dropbox listFolderContentsRecursively:dropboxRoot completion:^(NSArray *pathContents) {
            dispatch_async(_queue, ^{
                [self handleIncomingChanges:pathContents];
            });
        }];
    });
}

- (void)handleIncomingChanges:(NSArray *)fileInfos
{
    NSLog(@"Applying incoming changes from Dropbox...");
    for (DBFileInfo *fileInfo in fileInfos)
    {
        if ([_suppressedExtensions containsObject:[fileInfo.path.stringValue.pathExtension lowercaseString]])
        {
            continue;
        }
        NSString *localPath = [_path stringByAppendingPathComponent:fileInfo.path.stringValue];
        NSDate *fileModificationDate = _fileModificationDates[localPath];
        if (fileModificationDate)
        {
            // file exists locally
            if ([fileModificationDate compare:fileInfo.modifiedTime] == NSOrderedAscending)
            {
                NSLog(@"~~ incoming update: %@", localPath);
                [self updateFileToNewestVersion:fileInfo];
            }
        }
        else
        {
            // ignore files on Dropbox that have not been downloaded
        }
    }
    
    NSArray *files = [_fileModificationDates.allKeys copy];
    for (NSString *localPath in files)
    {
        NSString *dropboxPath = [localPath substringFromIndex:_path.length];
        BOOL found = NO;
        for (DBFileInfo *fileInfo in fileInfos)
        {
            if ([dropboxPath isEqualToString:fileInfo.path.stringValue])
            {
                found = YES;
                break;
            }
        }
        if (!found)
        {
            NSLog(@"-- incoming deletion: %@", localPath);
            [self stopWatchingFile:localPath];
            [_fileModificationDates removeObjectForKey:localPath];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            NSString *folder = [localPath stringByDeletingLastPathComponent];
            BOOL isDirectory = NO;
            if ([fileManager fileExistsAtPath:folder isDirectory:&isDirectory] && isDirectory &&
                [[fileManager contentsOfDirectoryAtPath:folder error:NULL] count] == 1)
            {
                [self stopWatchingFolder:folder];
                [fileManager removeItemAtPath:folder error:NULL];
            }
            else
            {
                [fileManager removeItemAtPath:localPath error:NULL];
            }
        }
    }
}

- (void)updateFileToNewestVersion:(DBFileInfo *)fileInfo
{
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    if (fileSystem == nil)
    {
        return;
    }
    NSString *localPath = [_path stringByAppendingPathComponent:fileInfo.path.stringValue];
    DBPath *path = [[DBPath root] childPath:fileInfo.path.stringValue];
    NSError *error = nil;
    DBFile *file = [fileSystem openFile:path error:&error];
    if (file == nil || error != nil)
    {
        NSLog(@"error opening file: %@", error != nil ? error : fileInfo.path.stringValue);
        return;
    }
    if (!file.newerStatus)
    {
        [file close];
        NSLog(@"== File is at newest version: %@", localPath);
        return;
    }
    [self stopWatchingFile:localPath];
    if (file.newerStatus.cached)
    {
        NSLog(@"== Newer version is cached, updating %@", localPath);
        [file update:NULL];
        [file close];
        
        [Dropbox copyFile:fileInfo.path.stringValue toPath:localPath];
        [self updateFileModificationDate:localPath];
        [self startWatchingFile:localPath];
        return;
    }
    [_downloadingFiles addObject:file]; // keep a strong reference
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:DropboxDownloadDidStartNotification object:fileInfo.path.stringValue];
    });
    __weak typeof(file) weakFile = file;
    [file addObserver:self block:^{
        if (weakFile.newerStatus.progress == 1)
        {
            if (weakFile.newerStatus.cached)
            {
                NSLog(@"== Newer version downloaded and cached, updating %@", localPath);
                [weakFile update:NULL];
                [weakFile close];
                
                [Dropbox copyFile:fileInfo.path.stringValue toPath:localPath];
                [self updateFileModificationDate:localPath];
                [self startWatchingFile:localPath];
                
                [_downloadingFiles removeObject:weakFile];
                if ([_downloadingFiles count] == 0)
                {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [[NSNotificationCenter defaultCenter] postNotificationName:DropboxDownloadDidStopNotification object:fileInfo.path.stringValue];
                    });
                }
            }
        }
    }];
}

- (void)updateFileModificationDate:(NSString *)localPath
{
    if (localPath == nil)
    {
        return;
    }
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSDictionary *attributes = [fileManager attributesOfItemAtPath:localPath error:NULL];
    if (attributes != nil)
    {
        [_fileModificationDates setObject:attributes.fileModificationDate forKey:localPath];
    }
    else
    {
        [_fileModificationDates removeObjectForKey:localPath];
    }
}

- (void)cacheAndRegisterFile:(NSString *)dropboxPath completion:(void(^)(NSString *localPath))completion
{
    dispatch_async(_queue, ^{
        NSString *localPath = [Dropbox fileSystemPath:dropboxPath];
        [_incomingFiles addObject:localPath];
        [Dropbox copyFile:dropboxPath];
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(localPath);
            });
        }
    });
}

- (void)handleLocalModification:(NSString *)path
{
    if (path == nil)
    {
        return;
    }
    NSDate *fileModificationDate = _fileModificationDates[path];
    if (fileModificationDate == nil)
    {
        // file not registered previously, ignore?
    }
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSDictionary *attributes = [fileManager attributesOfItemAtPath:path error:NULL];
    NSDate *newFileModificationDate = attributes ? attributes.fileModificationDate : nil;
    if (newFileModificationDate && [fileModificationDate compare:newFileModificationDate] == NSOrderedAscending)
    {
        NSLog(@"~~ uploading changed local file: %@", path);
        NSString *dropboxPath = [path substringFromIndex:_path.length];
        [Dropbox writeFile:path toPath:dropboxPath error:NULL];
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:DropboxWriteNotification object:path];
        });
    }
}

#pragma mark - TABFileMonitorDelegate

- (void)fileMonitor:(TABFileMonitor *)fileMonitor didSeeChange:(TABFileMonitorChangeType)changeType
{
    NSString *modification = nil;
    switch (changeType)
    {
        case TABFileMonitorChangeTypeModified:
            modification = @"modified";
            break;
//        case TABFileMonitorChangeTypeMetadata:
//            modification = @"metadata changed";
//            break;
//        case TABFileMonitorChangeTypeSize:
//            modification = @"size changed";
//            break;
        case TABFileMonitorChangeTypeRenamed:
            modification = @"renamed";
            break;
        case TABFileMonitorChangeTypeDeleted:
            modification = @"deleted";
            break;
//        case TABFileMonitorChangeTypeObjectLink:
//            modification = @"linked";
//            break;
        case TABFileMonitorChangeTypeRevoked:
            modification = @"revoked";
            break;
        default:
            break;
    }
    if (modification)
    {
        NSLog(@"** file %@: %@", modification, fileMonitor.path);
    }
    switch (changeType)
    {
        case TABFileMonitorChangeTypeModified:
            [self handleLocalModification:fileMonitor.path];
            break;
        default:
            break;
    }
}

@end
