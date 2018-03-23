//
//  OutputCellView.h
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookCellView.h"

extern const CGFloat OutputCellViewHorizontalContentMargin;

@class OutputCell;

@interface OutputCellView : NotebookCellView
{
@private
    __weak IBOutlet UILabel *_outLabel;
    __weak IBOutlet UIView *_textViewPlaceholderView;
    __weak IBOutlet UIImageView *_imageView;
    __weak IBOutlet NSLayoutConstraint *_textViewPlaceholderHeightConstraint;
}

+ (CGFloat)preferredHeightForCell:(OutputCell *)cell layoutWidth:(CGFloat)layoutWidth;

@end
