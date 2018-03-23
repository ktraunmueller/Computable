//
//  PythonEditorView.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonInputAccessoryView.h"
#import "../EditorView.h"

typedef NS_ENUM(NSUInteger, PythonEditorCommand)
{
    PythonEditorCommandRun,
};

@protocol PythonEditorDelegate;

@interface PythonEditorView : EditorView <PythonInputAccessoryDelegate>

+ (PythonEditorView *)newInstanceWithFrame:(CGRect)frame;

@end

@protocol PythonEditorViewDelegate <EditorViewDelegate>

@optional

- (void)pythonEditorViewDidRequestCommand:(PythonEditorCommand)command;

@end
