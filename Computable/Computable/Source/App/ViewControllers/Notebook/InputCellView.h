//
//  InputCellView.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookCellView.h"

extern NSString *const InputCellSourceEmptyDidChangeNotification;

extern const CGFloat InputCellViewHorizontalContentMargin;

@class InputCell;

@interface InputCellView : NotebookCellView
{
@private
    __weak IBOutlet UIButton *_infoButton;
    __weak IBOutlet UILabel *_inLabel;
    __weak IBOutlet UIView *_editorPlaceholderView;
    __weak IBOutlet NSLayoutConstraint *_editorPlaceholderHeightConstraint;
    __weak IBOutlet UITextView *_streamOutputView;
    __weak IBOutlet NSLayoutConstraint *_streamOutputHeightConstraint;
    __weak IBOutlet NSLayoutConstraint *_topMarginConstraint;
    __weak IBOutlet NSLayoutConstraint *_bottomMarginConstraint;
    __weak IBOutlet NSLayoutConstraint *_infoButtonTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_infoButtonLeadingConstraint;
}

+ (CGFloat)preferredHeightForCell:(InputCell *)cell sourceEditing:(BOOL)sourceEditing layoutWidth:(CGFloat)layoutWidth;

- (void)detachFromInputCell;

- (void)insertText:(NSString *)text;

- (IBAction)showCompletions:(id)sender;

@end
