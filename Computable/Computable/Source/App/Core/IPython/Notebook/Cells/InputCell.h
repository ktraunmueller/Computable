//
//  InputCell.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Cell.h"

extern NSString *const InputCellDidChangeTypeNotification;

typedef NS_ENUM(NSUInteger, InputCellType)
{
    InputCellTypeUnknown,
    InputCellTypeRaw,
    InputCellTypeCode,
    InputCellTypeMarkdown,
    InputCellTypeHeading,
};

@class OutputCell;

@interface InputCell : Cell

@property (nonatomic, assign) InputCellType type;
@property (nonatomic, readonly) BOOL isMarkdown;
@property (nonatomic, copy) NSString *source;
@property (nonatomic, assign) NSUInteger headingLevel;
@property (nonatomic, copy) NSString *language;
@property (nonatomic, strong) NSDictionary *metadata;
// rendering
@property (nonatomic, assign) NSRange selectedRange;
@property (nonatomic, assign) BOOL focused; // observable
@property (nonatomic, assign) BOOL editing; // observable
@property (nonatomic, assign) BOOL preventResignFirstResponderOnEndEditing;
@property (nonatomic, assign) BOOL preventEndEditingOnResignFirstResponder;
@property (nonatomic, assign) BOOL executing; // observable
@property (nonatomic, readonly) BOOL dirty; // observable
@property (nonatomic, readonly) NSString *markdownHtml;
@property (nonatomic, readonly) BOOL hasMath;
// execution
@property (nonatomic, copy) NSString *streamOut;
@property (nonatomic, strong) NSMutableArray *outputs;
@property (nonatomic, assign) BOOL flashStream;
@property (nonatomic, readonly) NSArray *responses;
@property (nonatomic, readonly) Message *executeReply;
@property (nonatomic, readonly) Message *errorResponse;
@property (nonatomic, strong) OutputCell *outputCell;
// layout caching
@property (nonatomic, assign) CGFloat cachedSourceHeight;
@property (nonatomic, strong) UIImage *renderedMarkdownImage;
@property (nonatomic, strong) NSArray *hyperlinks;

- (id)initWithNotebook:(Notebook *)notebook type:(InputCellType)type;

- (void)execute:(dispatch_block_t)completion;

- (void)resetDirty;

@end
