//
//  Notebook.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "../Session/Message.h"

@class Kernel;
@class InputCell;

@interface Notebook : NSObject

@property (nonatomic, copy) NSString *path;
@property (nonatomic, readonly) NSUInteger formatMajor;
@property (nonatomic, readonly) NSUInteger formatMinor;
@property (nonatomic, readonly) NSString *format;
@property (nonatomic, readonly) NSDictionary *metadata;
@property (nonatomic, readonly) Kernel *kernel;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, readonly) NSArray *cells;
@property (nonatomic, strong) InputCell *focusedInputCell;    // observable
@property (nonatomic, readonly) NSUInteger focusedInputCellIndex;

+ (Notebook *)newInstanceWithKernel:(Kernel *)kernel path:(NSString *)path notebookId:(NSString *)notebookId;

+ (NSString *)parseStrings:(NSDictionary *)json forKey:(NSString *)key;
+ (NSArray *)splitIntoLines:(NSString *)source;

- (void)loadWithOutputCells:(BOOL)createOutputCells;
- (void)save:(BOOL)force;

- (void)execute:(InputCell *)inputCell completion:(RequestBlock)completion;
- (void)interruptKernel;

- (InputCell *)newInputCell;
- (InputCell *)newInputCellBelowFocusedCell;
- (void)deleteFocusedInputCell;
- (void)deleteOutputCellForFocusedInputCell;

- (NSUInteger)moveFocusedCellUp;
- (NSUInteger)moveFocusedCellDown;
- (BOOL)isFirstInputCell:(InputCell *)inputCell;
- (BOOL)isLastInputCell:(InputCell *)inputCell;
- (BOOL)isLastMarkdownInputCell:(InputCell *)inputCell;
- (InputCell *)nextCodeInputCell:(InputCell *)inputCell;

- (NSString *)combinedSourceUpToAndIncluding:(InputCell *)inputCell;

- (void)invalidateCells;

@end
