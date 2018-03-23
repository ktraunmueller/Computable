//
//  IPythonApplication.h
//  Computable
//
//  Created by Karl Traunmüller on 02.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Notebook;
@class Kernel;

extern NSString *const kNotebookExtension;

extern NSString *const NotebookApplicationDidStartNotification;
extern NSString *const NotebookApplicationDidStopNotification;

@interface IPythonApplication : NSObject

+ (void)prepare:(dispatch_block_t)completion;
+ (void)start:(dispatch_block_t)completion;
+ (void)stop:(dispatch_block_t)completion;

+ (void)reloadNotebookFiles;
+ (NSArray *)notebookCategories;
+ (NSArray *)notebooksInCategory:(NSString *)category;
+ (NSString *)categoryForNotebook:(NSString *)notebookFile;
+ (BOOL)allowEditingCategory:(NSString *)category;
+ (NSString *)renameNotebookFromFile:(NSString *)fromFile toFile:(NSString *)toFile;
+ (BOOL)deleteNotebook:(NSString *)notebookFile;
+ (NSString *)thumbnailPathForNotebook:(NSString *)notebookFile;

+ (void)newNotebook:(void (^)(Notebook *notebook))completion;
+ (void)openNotebook:(NSString *)notebookPath completion:(void (^)(Notebook *notebook))completion;
+ (void)closeNotebook:(Notebook *)notebook completion:(dispatch_block_t)completion;
+ (NSArray *)openNotebooks;
+ (NSString *)copyExternalNotebook:(NSURL *)notebookUrl;

+ (void)doOneIteration:(Kernel *)kernel;
+ (void)interruptKernel:(Kernel *)kernel;

@end
