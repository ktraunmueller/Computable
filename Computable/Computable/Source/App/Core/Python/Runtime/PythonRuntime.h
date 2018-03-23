//
//  PythonRuntime.h
//  PyBooks-Common
//
//  Created by Karl Traunmüller on 17.01.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Python.h>

@interface PythonRuntime : NSObject

+ (PythonRuntime *)instance;
+ (NSString *)runtimePath;

+ (void)print:(char *)prefix object:(PyObject *)obj;

- (PyObject *)globals;

- (BOOL)needsRuntimeFolderSetup;
- (void)setupRuntimeFolder:(dispatch_block_t)completion;

- (void)launch:(dispatch_block_t)completion;
- (void)shutdown:(dispatch_block_t)completion;

- (void)execute:(dispatch_block_t)block;
- (void)execute:(dispatch_block_t)block completion:(dispatch_block_t)completion;
- (void)detachThread:(dispatch_block_t)block;

@end
