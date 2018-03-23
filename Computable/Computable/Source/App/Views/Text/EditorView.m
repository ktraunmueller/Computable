//
//  EditorView.m
//  Computable
//
//  Created by Karl Traunmüller on 03.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "EditorView.h"
#import "SimpleTextStorage.h"
#import "../../Core/Settings/Settings.h"
#import "../../../Common/Utilities/NSAttributedString+Dimension.h"
#import "../../../Common/Utilities/NSString+Dimension.h"

UIEdgeInsets EditorTextContainerInsets;

@implementation EditorView
{
    BOOL _acceptFirstResponder;
    NSRange _selectedRange;
    CGFloat _layoutHeight;
    CGPoint _trackingOffset;
}

#pragma mark - Statics

+ (void)initialize
{
    EditorTextContainerInsets = UIEdgeInsetsMake(4, 4, 4, 4);
}

#pragma mark - Properties
//
//- (void)setText:(NSString *)text
//{
//    [super setText:text];
//    
//    _layoutHeight = [self preferredHeight];
//    [self.editorDelegate editorViewHeightDidChange:self];
//}
//
//- (void)setAttributedText:(NSAttributedString *)attributedText
//{
//    [super setAttributedText:attributedText];
//    
//    _layoutHeight = [self preferredHeight];
//    [self.editorDelegate editorViewTextDidChange:self];
//}

#pragma mark - UITextView

- (id)initWithFrame:(CGRect)frame textContainer:(NSTextContainer *)textContainer
{
    if (self = [super initWithFrame:frame textContainer:textContainer])
    {
        self.delegate = self;
        self.autocapitalizationType = UITextAutocapitalizationTypeNone;
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - EditorView

- (CGFloat)preferredHeight
{
    CGFloat layoutWidth = CGRectGetWidth(self.bounds) - self.textContainerInset.left - self.textContainerInset.right;
    CGFloat vmargin = self.textContainerInset.top + self.textContainerInset.bottom;
    UIFont *font = [Settings instance].codeFont.regularFont;
    if (self.attributedText == nil)
    {
        return ceilf([self.text layoutHeightForWidth:layoutWidth withFont:font]) + vmargin;
    }
    return ceilf([self.attributedText layoutHeightForWidth:layoutWidth withFont:font]) + vmargin;
}

- (void)insertString:(NSString *)string
{
    if (![self isFirstResponder])
    {
        return;
    }
    [self.textStorage replaceCharactersInRange:self.selectedRange withString:string];
    self.selectedRange = NSMakeRange(self.selectedRange.location + 1, 0);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.editorDelegate editorViewTextDidChange:self];
    });
}

- (void)updateTextStorageWithString:(NSAttributedString *)string
{
    SimpleTextStorage *textStorage = (SimpleTextStorage *)self.textStorage;
    [textStorage updateWithString:string];
}

- (void)trackpadDidMoveCursor:(NSNotification *)notification
{
    if (![self isFirstResponder])
    {
        return;
    }
    CGPoint offset = [(NSValue *)notification.object CGPointValue];
    _trackingOffset.x += offset.x;
    _trackingOffset.y += offset.y;
    NSInteger dx = roundf(_trackingOffset.x / 15);
    NSInteger dy = roundf(_trackingOffset.y / 15);
    if (dx == dy)
    {
        return;
    }
    if (fabs(dx) > fabs(dy))
    {
        _trackingOffset.x = 0;
        _trackingOffset.y = 0;
        NSInteger location = self.selectedRange.location;
        self.selectedRange = NSMakeRange(MAX(0, location + dx), 0);
    }
    else
    {
        NSRange lineRange = [self.textStorage.string lineRangeForRange:self.selectedRange];
        NSUInteger locationInLine = self.selectedRange.location - lineRange.location;
        if (dy > 0)
        {
            _trackingOffset.x = 0;
            _trackingOffset.y = 0;
            lineRange = NSMakeRange(lineRange.location + lineRange.length, 0);
            lineRange = [self.textStorage.string lineRangeForRange:lineRange];
            self.selectedRange = NSMakeRange(MIN(lineRange.location + locationInLine, lineRange.location + lineRange.length - 1), 0);
        }
        else if (dy < 0 && lineRange.location > 0)
        {
            _trackingOffset.x = 0;
            _trackingOffset.y = 0;
            lineRange = NSMakeRange(lineRange.location - 1, 0);
            lineRange = [self.textStorage.string lineRangeForRange:lineRange];
            self.selectedRange = NSMakeRange(MIN(lineRange.location + locationInLine, lineRange.location + lineRange.length - 1), 0);
        }
    }
}

#pragma mark - UIResponder

- (BOOL)becomeFirstResponder
{
    BOOL accept = [self.editorDelegate editorViewWantsBecomeFirstResponder:self];
    if (accept)
    {
        [super becomeFirstResponder];
    }
    return accept;
}

- (BOOL)resignFirstResponder
{
    [self.editorDelegate editorViewWantsResignFirstResponder:self];
    
    return [super resignFirstResponder];
}

#pragma mark - UITextViewDelegate

- (void)textViewDidBeginEditing:(UITextView *)textView
{
    textView.scrollEnabled = NO;    // seems to fix http://localhost:8000/computable/ticket/86
    textView.scrollEnabled = YES;
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
}

- (void)textViewDidChangeSelection:(UITextView *)textView
{
    _selectedRange = self.selectedRange;
    
    if ([textView isFirstResponder])
    {
        [self.editorDelegate editorViewTextDidChange:self]; // for completions
    }
}

- (void)textViewDidChange:(UITextView *)textView
{
    _selectedRange = self.selectedRange;
    
    [self.editorDelegate editorViewTextDidChange:self];
    
    CGFloat newHeight = [self preferredHeight];
    BOOL heightDidChange = newHeight != _layoutHeight;
    _layoutHeight = newHeight;
    if (heightDidChange)
    {
        textView.scrollEnabled = NO;    // seems to fix http://localhost:8000/computable/ticket/86
        [self.editorDelegate editorViewHeightDidChange:self];
        textView.scrollEnabled = YES;
        textView.contentOffset = CGPointZero;
    }
}

@end
