//
//  Dropbox.h
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Dropbox/Dropbox.h>

extern NSString *const DropboxFolder;
extern NSString *kDropboxUrlScheme;

typedef void(^DropboxListCompletion)(NSArray *pathContents);

@class DBPath;

@interface Dropbox : NSObject

+ (NSString *)userHomeRelativePath:(NSString *)dropboxPath;

+ (void)setup;
+ (BOOL)isLinked;
+ (NSString *)username;

+ (BOOL)openURL:(NSURL *)url;

+ (void)listFolderContents:(DBPath *)path completion:(void (^)(NSArray *pathContents))completion;
+ (void)listFolderContentsRecursively:(DBPath *)path completion:(void (^)(NSArray *pathContents))completion;

+ (void)writeFile:(NSString *)fileSystemPath toPath:(NSString *)dropboxPath error:(DBError **)error;
+ (void)copyFile:(NSString *)dropboxPath;
+ (void)copyFile:(NSString *)dropboxPath toPath:(NSString *)fileSystemPath;
+ (void)deleteFile:(NSString *)dropboxPath;
+ (BOOL)fileExistsLocally:(NSString *)dropboxPath;
+ (NSString *)fileSystemPath:(NSString *)dropboxPath;

@end
