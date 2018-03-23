//
//  EditorView.h
//  Computable
//
//  Created by Karl Traunmüller on 03.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern UIEdgeInsets EditorTextContainerInsets;

@protocol EditorViewDelegate;
@protocol EditorViewCompletionDelegate;

@interface EditorView : UITextView <UITextViewDelegate>

@property (nonatomic, weak) id<EditorViewDelegate> editorDelegate;
@property (nonatomic, copy) NSString *placeholderString;

- (CGFloat)preferredHeight;
- (void)insertString:(NSString *)string;

- (void)trackpadDidMoveCursor:(NSNotification *)notification;

@end

@protocol EditorViewDelegate <NSObject>

- (BOOL)editorViewWantsBecomeFirstResponder:(EditorView *)editorView;
- (BOOL)editorViewWantsResignFirstResponder:(EditorView *)editorView;
- (void)editorViewTextDidChange:(EditorView *)editorView;
- (void)editorViewHeightDidChange:(EditorView *)editorView;

@end