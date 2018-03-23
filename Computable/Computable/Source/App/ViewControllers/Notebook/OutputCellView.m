//
//  OutputCellView.m
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "OutputCellView.h"
#import "../../Core/IPython/Notebook/Cells/OutputCell.h"
#import "../../Core/IPython/Session/Message.h"
#import "../../Core/Settings/Settings.h"
#import "../../Views/Text/EditorView.h"
#import "../../../Common/Utilities/NSString+Dimension.h"
#import "../../../Common/Utilities/UIView+LayoutDiagnostics.h"

const CGFloat OutputCellViewHorizontalContentMargin = 74;

static const CGFloat kVerticalContentMargin = 4;
static const NSTimeInterval kFlashDuration = 1.0;

static UIEdgeInsets TextViewInsets;

@implementation OutputCellView
{
    UITextView *_textView;
}

#pragma mark - Statics

+ (void)initialize
{
    TextViewInsets = UIEdgeInsetsMake(4, 4, 4, 4);
}

+ (CGFloat)preferredOutHeightForCell:(OutputCell *)cell layoutWidth:(CGFloat)layoutWidth
{
    CGFloat outputWidth = layoutWidth - OutputCellViewHorizontalContentMargin;
    // TODO handle combined image/text output?
    if (cell.imageData != nil)
    {
        CGSize imageSize = cell.imageData.size;
        CGFloat scale = 1.0;
        if (imageSize.width > outputWidth)
        {
            scale = outputWidth / imageSize.width;
        }
        return imageSize.height * scale + 8;    // leave some margin
    }
    NSString *text = cell.textData;
    if ([text length] == 0)
    {
        text = @"_";
    }
    CGFloat vmargin = EditorTextContainerInsets.top + EditorTextContainerInsets.bottom;
    UIFont *font = [Settings instance].codeFont.regularFont;
    CGFloat height = roundf([text layoutHeightForWidth:outputWidth withFont:font]) + vmargin;
    return height;
}

+ (CGFloat)preferredHeightForCell:(OutputCell *)cell layoutWidth:(CGFloat)layoutWidth
{
    if (cell == nil)
    {
        return 0;
    }
    CGFloat outHeight = [OutputCellView preferredOutHeightForCell:cell layoutWidth:layoutWidth];
    return outHeight > 0 ? outHeight + 2 * kVerticalContentMargin : 1;
}

#pragma mark - Properties

- (void)setCell:(Cell *)cell
{
    [super setCell:cell];
    
    OutputCell *outputCell = (OutputCell *)cell;
    if (outputCell.executionCount > 0)
    {
        _outLabel.text = [NSString stringWithFormat:@"Out[%d]", (int)outputCell.executionCount];
    }
    else
    {
        _outLabel.text = @"Out[]";
    }
    
    _textView.text = outputCell.textData;
    _imageView.image = outputCell.imageData;
    _imageView.hidden = outputCell.imageData == nil;
    _textView.hidden = !_imageView.hidden;
    
    CGFloat layoutWidth = CGRectGetWidth(self.bounds);
    CGFloat outputWidth = layoutWidth - OutputCellViewHorizontalContentMargin;
    if (outputCell.imageData != nil)
    {
        _imageView.contentMode = outputCell.imageData.size.width > outputWidth ? UIViewContentModeScaleAspectFit : UIViewContentModeLeft;
    }
    _textViewPlaceholderHeightConstraint.constant = [OutputCellView preferredOutHeightForCell:self.outputCell layoutWidth:layoutWidth];
    
    if (outputCell.flash)
    {
        outputCell.flash = NO;
        _textView.backgroundColor = [[UIColor yellowColor] colorWithAlphaComponent:0.3];
        [UIView animateWithDuration:kFlashDuration
                         animations:^{
                             _textView.backgroundColor = [UIColor whiteColor];
                         }];
    }
}

//- (CGSize)outputAreaSize
//{
//    return _textViewPlaceholderView.frame.size;
//}

#pragma mark - OutputCellView

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Notifications

- (void)fontSettingsDidChange:(NSNotification *)notification
{
    _outLabel.font = [Settings instance].codeFont.regularFont;
    _textView.font = [Settings instance].codeFont.regularFont;
}

#pragma mark - NSObject(UINibLoadingAdditions)

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    self.backgroundColor = [UIColor whiteColor];
//    self.layer.borderWidth = 1;
//    self.layer.borderColor = [[UIColor redColor] CGColor];
    
    _outLabel.font = [Settings instance].codeFont.regularFont;
    
    NSTextStorage *textStorage = [NSTextStorage new];
    NSLayoutManager *layoutManager = [NSLayoutManager new];
    [textStorage addLayoutManager:layoutManager];
    NSTextContainer *textContainer = [[NSTextContainer alloc] initWithSize:_textViewPlaceholderView.bounds.size];
    [layoutManager addTextContainer:textContainer];
    _textView = [[UITextView alloc] initWithFrame:_textViewPlaceholderView.bounds textContainer:textContainer];
    _textView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [_textViewPlaceholderView addSubview:_textView];
    
    _textView.editable = NO;
    _textView.spellCheckingType = UITextSpellCheckingTypeNo;
    _textView.font = [Settings instance].codeFont.regularFont;
    _textView.backgroundColor = [UIColor whiteColor];
    _textView.textContainerInset = TextViewInsets;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(fontSettingsDidChange:)
                                                 name:FontSettingsDidChangeNotification
                                               object:nil];
}

@end
