//
//  NotebookCell.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "InputCellView.h"
#import "NotebookViewController.h"
#import "CompletionsViewController.h"
#import "../../Colors.h"
#import "../../Core/IPython/Notebook/Notebook.h"
#import "../../Core/IPython/Notebook/Cells/InputCell.h"
#import "../../Core/IPython/Session/Message.h"
#import "../../Core/Markdown/MarkdownHtmlRenderer.h"
#import "../../Core/Python/Editor/PythonEditor.h"
#import "../../Core/Settings/Settings.h"
#import "../../Views/Html/Hyperlink.h"
#import "../../Views/Html/HyperlinkView.h"
#import "../../Views/Text/EditorView.h"
#import "../../Views/Text/Markdown/MarkdownEditorView.h"
#import "../../Views/Text/Python/PythonEditorView.h"
#import "../../../Common/Utilities/NSString+Dimension.h"
#import "../../../Common/Utilities/NSString+Location.h"

NSString *const InputCellSourceEmptyDidChangeNotification = @"InputCellSourceEmptyDidChangeNotification"; // allow modifying type only if source empty

const CGFloat InputCellViewHorizontalContentMargin = 74;

static const CGFloat kVerticalContentMargin = 4;
static const CGFloat kCompletionMargin = 40;

static const NSTimeInterval kFlashDuration = 1.0;
static const NSTimeInterval kFadeDuration = 0.25;

static UIColor *BorderColor;
static UIColor *FocusedBorderColor;

@interface InputCellView () <EditorViewDelegate, PythonEditorViewDelegate>
@end

@implementation InputCellView
{
    BOOL _hasRegisteredKVO;
    BOOL _suppressStartEditingOnKVO, _suppressEndEditingOnKVO;
    UIImageView *_markdownView;
    MarkdownEditorView *_markdownSourceEditorView;
    PythonEditorView *_pythonEditorView;
    UITextView *_placeholderTextView;
    BOOL _showCompletionsPopup;
    CGPoint _cursorPosition;
    TextLocation _caretLocation;
    CGPoint _infoButtonCenter;
}

#pragma mark - Statics

+ (void)initialize
{
    BorderColor = [UIColor colorWithWhite:0.92 alpha:1.0];
    FocusedBorderColor = [UIColor colorWithWhite:0.85 alpha:1.0];
}

+ (CGFloat)preferredCodeHeightForCell:(InputCell *)cell sourceEditing:(BOOL)sourceEditing layoutWidth:(CGFloat)layoutWidth
{
    if (cell.isMarkdown)
    {
        if (!sourceEditing)
        {
            return cell.renderedMarkdownImage.size.height;
        }
    }
    if (cell.cachedSourceHeight > 0)
    {
        return cell.cachedSourceHeight;
    }
    
    layoutWidth -= InputCellViewHorizontalContentMargin + EditorTextContainerInsets.left + EditorTextContainerInsets.right;
    CGFloat vmargin = EditorTextContainerInsets.top + EditorTextContainerInsets.bottom;
    CGFloat height = 0;
    UIFont *codeFont = [Settings instance].codeFont.regularFont;
    if (cell.source.length == 0)
    {
        height = ceilf(codeFont.lineHeight) + vmargin;
    }
    else
    {
        height = ceilf([cell.source layoutHeightForWidth:layoutWidth withFont:codeFont] + (cell.isMarkdown ? codeFont.lineHeight : 0)) + vmargin;
    }
    cell.cachedSourceHeight = height;
    return height;
}

+ (CGFloat)preferredOutputHeightForCell:(InputCell *)cell layoutWidth:(CGFloat)layoutWidth
{
    layoutWidth -= InputCellViewHorizontalContentMargin + EditorTextContainerInsets.left + EditorTextContainerInsets.right;
    CGFloat vmargin = EditorTextContainerInsets.top + EditorTextContainerInsets.bottom;
    NSString *text = cell.errorResponse ? cell.errorResponse.errorValue : cell.streamOut;
    if (text.length == 0)
    {
        return 0;
    }
    UIFont *codeFont = [Settings instance].codeFont.regularFont;
    CGFloat height = ceilf([text layoutHeightForWidth:layoutWidth withFont:codeFont]) + vmargin;
    return height;
}

+ (CGFloat)preferredHeightForCell:(InputCell *)cell sourceEditing:(BOOL)sourceEditing layoutWidth:(CGFloat)layoutWidth
{
    CGFloat codeHeight = [InputCellView preferredCodeHeightForCell:cell sourceEditing:sourceEditing layoutWidth:layoutWidth];
    CGFloat outputHeight = [InputCellView preferredOutputHeightForCell:cell layoutWidth:layoutWidth];
    CGFloat height = kVerticalContentMargin + codeHeight + outputHeight;
    return height;
}

#pragma mark - Properties

- (void)setCell:(Cell *)cell
{
    if (self.inputCell)
    {
        [self unregisterKVO];
    }
    [super setCell:cell];
    if (self.inputCell)
    {
        [self registerKVO];
    }
    
    cell.context = self;
    
    _infoButton.hidden = YES;
    _showCompletionsPopup = NO;
    
    _inLabel.text = self.inputCell.executionCount > 0 ? [NSString stringWithFormat:@"In [%d]", (int)self.inputCell.executionCount] : @"In []";
    _inLabel.hidden = self.inputCell.isMarkdown;
    _markdownView.image = nil;
    
    _markdownSourceEditorView.editorDelegate = self;
    _pythonEditorView.editorDelegate = self;
    
    [self updateInputViews];
    
    if (self.inputCell.errorResponse)
    {
        _streamOutputView.textColor = [UIColor redColor];
        _streamOutputView.text = self.inputCell.errorResponse.errorValue;
    }
    else
    {
        _streamOutputView.textColor = [UIColor blackColor];
        _streamOutputView.text = self.inputCell.streamOut;
    }
    CGFloat layoutWidth = CGRectGetWidth(self.bounds);
    _streamOutputHeightConstraint.constant = [InputCellView preferredOutputHeightForCell:self.inputCell
                                                                             layoutWidth:layoutWidth];
    _editorPlaceholderHeightConstraint.constant = [InputCellView preferredCodeHeightForCell:self.inputCell
                                                                              sourceEditing:self.inputCell.editing
                                                                                layoutWidth:layoutWidth];
    
    if (self.inputCell.flashStream)
    {
        self.inputCell.flashStream = NO;
        _streamOutputView.backgroundColor = [[UIColor yellowColor] colorWithAlphaComponent:0.3];
        [UIView animateWithDuration:kFlashDuration
                         animations:^{
                             _streamOutputView.backgroundColor = [UIColor clearColor];
                         }];
    }
}

#pragma mark - InputCellView

- (void)dealloc
{
    [self unregisterKVO];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)detachFromInputCell
{
    _pythonEditorView.editorDelegate = nil;
    _markdownSourceEditorView.editorDelegate = nil;
    [self unregisterKVO];
}

- (void)unregisterKVO
{
    //    NSLog(@"InputCellView %p unregisterKVO on %@", self, self.inputCell);
    
    if (!_hasRegisteredKVO)
    {
        return;
    }
    _hasRegisteredKVO = NO;
    
    @try
    {
        [self.inputCell removeObserver:self forKeyPath:@"focused"];
        [self.inputCell removeObserver:self forKeyPath:@"editing"];
        [self.inputCell removeObserver:self forKeyPath:@"executing"];
        [self.inputCell removeObserver:self forKeyPath:@"type"];
    }
    @catch (NSException * __unused exception)
    {
        // ignore
    }
}

- (void)registerKVO
{
    //    NSLog(@"InputCellView %p registerKVO on %@", self, self.inputCell);
    
    if (_hasRegisteredKVO)
    {
        return;
    }
    _hasRegisteredKVO = YES;
    
    [self.inputCell addObserver:self forKeyPath:@"focused" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
    [self.inputCell addObserver:self forKeyPath:@"editing" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
    [self.inputCell addObserver:self forKeyPath:@"executing" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
    [self.inputCell addObserver:self forKeyPath:@"type" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
}

- (void)updateInputViews
{
    if (self.inputCell.isMarkdown)
    {
        _markdownView.image = self.inputCell.renderedMarkdownImage;
        [self updateHyperlinks];
        
        _markdownSourceEditorView.editorDelegate = nil;
        _markdownSourceEditorView.text = self.inputCell.source;
        _markdownSourceEditorView.editorDelegate = self;
        if (self.inputCell.editing)
        {
            if ([_editorPlaceholderView.subviews lastObject] != _markdownSourceEditorView)
            {
                [_editorPlaceholderView bringSubviewToFront:_markdownSourceEditorView];
            }
        }
        else
        {
            if ([_editorPlaceholderView.subviews lastObject] != _markdownView)
            {
                [_editorPlaceholderView bringSubviewToFront:_markdownView];
            }
        }
    }
    else
    {
        _pythonEditorView.editorDelegate = nil;
        _pythonEditorView.text = self.inputCell.source;
        _pythonEditorView.editorDelegate = self;
        if ([_editorPlaceholderView.subviews lastObject] != _pythonEditorView)
        {
            [_editorPlaceholderView bringSubviewToFront:_pythonEditorView];
        }
    }
    _pythonEditorView.layer.borderColor = self.inputCell.focused ? [FocusedBorderColor CGColor] : [BorderColor CGColor];
    _markdownSourceEditorView.layer.borderColor = self.inputCell.focused ? [FocusedBorderColor CGColor] : [BorderColor CGColor];
    
    _placeholderTextView.text = self.inputCell.isMarkdown ? NSLocalizedString(@"Markdown cell", nil) : NSLocalizedString(@"Code cell", nil);
    if (self.inputCell.source.length == 0)
    {
        [_editorPlaceholderView bringSubviewToFront:_placeholderTextView];
        _placeholderTextView.alpha = 1;
    }
    else
    {
        [_editorPlaceholderView sendSubviewToBack:_placeholderTextView];
    }
}

- (void)updateHyperlinks
{
    [_markdownView.subviews makeObjectsPerformSelector:@selector(removeFromSuperview)];
    
    for (Hyperlink *link in self.inputCell.hyperlinks)
    {
        for (NSValue *value in link.clientRects)
        {
            CGRect rect = [value CGRectValue];
            HyperlinkView *view = [[HyperlinkView alloc] initWithURL:link.url frame:rect];
            [_markdownView addSubview:view];
        }
    }
}

- (void)execute
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.notebookController executeCell:self.inputCell];
    });
}

- (void)showStar:(BOOL)show
{
    _inLabel.text = show ? @"In [*]" : @"In []";
}

- (void)startEditing
{
    if (self.inputCell.type == InputCellTypeCode)
    {
        [_pythonEditorView becomeFirstResponder];
        _pythonEditorView.selectedRange = self.inputCell.selectedRange;
    }
    else
    {
        [self beginEditingMarkdownAnimated:YES];
    }
}

- (void)endEditing
{
    if (_showCompletionsPopup)
    {
        _showCompletionsPopup = NO;
        [[CompletionsViewController instance] dismissAnimated:YES];
    }
    [self hideInfoButton];
    
    if (self.inputCell.type == InputCellTypeCode)
    {
        self.inputCell.selectedRange = _pythonEditorView.selectedRange;
        if (!self.inputCell.editing && [_pythonEditorView isFirstResponder])
        {
            if (self.inputCell.preventResignFirstResponderOnEndEditing)
            {
                self.inputCell.preventResignFirstResponderOnEndEditing = NO;
            }
            else
            {
                [_pythonEditorView resignFirstResponder];
            }
        }
    }
    else
    {
        [self endEditingMarkdownAnimated:YES];
    }
}

- (void)beginEditingMarkdownAnimated:(BOOL)animated
{
    if (!self.inputCell.editing || [_markdownSourceEditorView isFirstResponder])
    {
        return;
    }
    _markdownSourceEditorView.layer.borderColor = [FocusedBorderColor CGColor];
    _markdownSourceEditorView.editorDelegate = nil;
    _markdownSourceEditorView.text = self.inputCell.source;
    _markdownSourceEditorView.editorDelegate = self;
    _markdownSourceEditorView.alpha = 0;
    [_editorPlaceholderView bringSubviewToFront:_markdownView];
    [_editorPlaceholderView bringSubviewToFront:_markdownSourceEditorView];
    
    [UIView animateWithDuration:animated && self.inputCell.source.length > 0 ? kFadeDuration : 0
                     animations:^{
                         _markdownView.alpha = 0;
                         _markdownSourceEditorView.alpha = 1;
                     } completion:^(BOOL finished) {
                         if (!finished)
                         {
                             return;
                         }
                         if (self.inputCell.source.length == 0)
                         {
                             [_editorPlaceholderView bringSubviewToFront:_placeholderTextView];
                         }
                         else
                         {
                             _markdownSourceEditorView.font = [Settings instance].codeFont.regularFont;
                             [_editorPlaceholderView bringSubviewToFront:_markdownSourceEditorView];
                         }
                         [self editorViewHeightDidChange:_markdownSourceEditorView]; // size to source height
                         if (![_markdownSourceEditorView isFirstResponder])
                         {
                             self.inputCell.preventEndEditingOnResignFirstResponder = YES;
                             [_markdownSourceEditorView becomeFirstResponder];
                         }
                         //                         if (self.inputCell.selectedRange.location != NSNotFound)
                         //                         {
                         //                             _markdownSourceEditorView.selectedRange = self.inputCell.selectedRange;
                         //                         }
                     }];
}

- (void)endEditingMarkdownAnimated:(BOOL)animated
{
    self.inputCell.selectedRange = _markdownSourceEditorView.selectedRange;
    if (!self.inputCell.editing && [_markdownSourceEditorView isFirstResponder])
    {
        if (self.inputCell.preventResignFirstResponderOnEndEditing)
        {
            self.inputCell.preventResignFirstResponderOnEndEditing = NO;
        }
        else
        {
            [_markdownSourceEditorView resignFirstResponder];
        }
    }
    if (self.inputCell.source.length == 0)
    {
        self.inputCell.source = NSLocalizedString(@"Empty Markdown cell.", nil);
    }
    
    [UIView animateWithDuration:animated ? kFadeDuration : 0
                     animations:^{
                         _markdownSourceEditorView.alpha = 0;
                         _markdownView.alpha = 1;
                     } completion:^(BOOL finished) {
                         if (!finished)
                         {
                             return;
                         }
                         [_editorPlaceholderView bringSubviewToFront:_markdownView];
                         _markdownSourceEditorView.alpha = 1;
                     }];
}

- (void)insertText:(NSString *)text
{
    self.inputCell.source = [self.inputCell.source stringByReplacingCharactersInRange:self.inputCell.selectedRange withString:text];
    if (self.inputCell.type == InputCellTypeCode)
    {
        _pythonEditorView.editorDelegate = nil;
        _pythonEditorView.text = self.inputCell.source;
        _pythonEditorView.editorDelegate = self;
    }
    else
    {
        _markdownSourceEditorView.editorDelegate = nil;
        _markdownSourceEditorView.text = self.inputCell.source;
        _markdownSourceEditorView.editorDelegate = self;
    }
}

- (BOOL)infoButtonHidden
{
    return _infoButton.hidden || _infoButton.alpha == 0;
}

- (void)showInfoButton
{
    if (_infoButtonCenter.x == 0 && _infoButtonCenter.y == 0)
    {
        return;
    }
    NSRange range = _pythonEditorView.selectedRange;
    BOOL suppressInfoButton = NO;
    if (range.location != NSNotFound && range.length == 0)
    {
        if (range.location > 0 && range.location < self.inputCell.source.length)
        {
            NSString *characterLeftOfCaret = [self.inputCell.source substringWithRange:NSMakeRange(range.location - 1, 1)];
            suppressInfoButton = [characterLeftOfCaret stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]].length == 0;
        }
        if (suppressInfoButton)
        {
            [self hideInfoButton];
            return;
        }
    }
    if (![self infoButtonHidden])
    {
        return;
    }
    _infoButton.alpha = 0;
    _infoButton.hidden = NO;
    
    [UIView animateWithDuration:kFadeDuration
                          delay:0
         usingSpringWithDamping:0.8
          initialSpringVelocity:0
                        options:0
                     animations:^{
                         _infoButton.alpha = 1;
                     } completion:nil];
}

- (void)hideInfoButton
{
    [UIView animateWithDuration:kFadeDuration
                          delay:0
         usingSpringWithDamping:0.8
          initialSpringVelocity:0
                        options:0
                     animations:^{
                         _infoButton.alpha = 0;
                     } completion:nil];
}

- (void)moveInfoButtonToCaretLocation
{
    if (isnan(_infoButtonCenter.x) || isnan(_infoButtonCenter.y) ||
        isinf(_infoButtonCenter.x) || isinf(_infoButtonCenter.y))
    {
        return;
    }
    _infoButtonLeadingConstraint.constant = _infoButtonCenter.x - CGRectGetWidth(_infoButton.frame) / 2;
    _infoButtonTopConstraint.constant = _infoButtonCenter.y - CGRectGetHeight(_infoButton.frame) / 2;
    [UIView animateWithDuration:[self infoButtonHidden] ? 0 : kFadeDuration
                          delay:0
         usingSpringWithDamping:0.8
          initialSpringVelocity:0
                        options:0
                     animations:^{
                         [_infoButton.superview layoutIfNeeded];
                     } completion:nil];
}

- (void)computeCaretLocation
{
    _caretLocation.line = NSNotFound;
    _caretLocation.column = NSNotFound;
    
    NSRange range = _pythonEditorView.selectedRange;
    if (range.location != NSNotFound && range.length == 0)
    {
        _caretLocation = [self.inputCell.source locationForRange:range];
        if (_caretLocation.line != NSNotFound && _caretLocation.column != NSNotFound)
        {
            CGPoint caretOrigin = [_pythonEditorView caretRectForPosition:_pythonEditorView.selectedTextRange.start].origin;
            if (isinf(caretOrigin.x) || isinf(caretOrigin.y) ||
                isnan(caretOrigin.x) || isnan(caretOrigin.y))
            {
                return;
            }
            _cursorPosition = caretOrigin;
            CGPoint myCursorPosition = [self convertPoint:_cursorPosition fromView:_pythonEditorView];
            _infoButtonCenter = CGPointMake(roundf(myCursorPosition.x) + kCompletionMargin,
                                            roundf(myCursorPosition.y + _pythonEditorView.font.lineHeight / 2) + 1);
        }
    }
}

- (void)showCompletionsPopup
{
    CGRect popupRect = CGRectMake(_cursorPosition.x - kCompletionMargin, _cursorPosition.y,
                                  2 * kCompletionMargin, _pythonEditorView.font.lineHeight); // update position
    if (![CompletionsViewController instance].popoverVisible)
    {
        [[CompletionsViewController instance] showCompletions:@[ kLoadingCompletion ]
                                                     fromRect:popupRect
                                                       inView:_pythonEditorView];
    }
    else
    {
        [[CompletionsViewController instance] movePopoverToRect:popupRect inView:_pythonEditorView];
    }
    NSString *combinedSource = [self.inputCell.notebook combinedSourceUpToAndIncluding:self.inputCell];
    NSUInteger totallines = [[combinedSource componentsSeparatedByString:@"\n"] count];
    NSUInteger cellLines = [[self.inputCell.source componentsSeparatedByString:@"\n"] count];
    [PythonEditor completionsForSource:combinedSource
                                atLine:_caretLocation.line + totallines - cellLines
                                column:_caretLocation.column
                                result:^(NSArray *completions) {
//                                    NSLog(@"%@", completions);
                                   CGRect popupRect = CGRectMake(_cursorPosition.x - kCompletionMargin, _cursorPosition.y,
                                                                 2 * kCompletionMargin, _pythonEditorView.font.lineHeight);
                                    [[CompletionsViewController instance] showCompletions:completions
                                                                                 fromRect:popupRect
                                                                                   inView:_pythonEditorView];
                                }];
}

- (void)showCompletions:(id)sender
{
    [self hideInfoButton];
    _showCompletionsPopup = YES;
    [self showCompletionsPopup];
}

- (void)completionsPopupDismissed
{
    _showCompletionsPopup = NO;
    [self showInfoButton];
    [self moveInfoButtonToCaretLocation];
}

#pragma mark - Gestures

- (void)markdownTapped:(UIGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        if (self.inputCell.focused)
        {
            self.inputCell.editing = YES;
        }
        else
        {
            self.inputCell.notebook.focusedInputCell = self.inputCell;
        }
    }
}

#pragma mark - Notifications

- (void)completionsPopupDidSelectCompletion:(NSNotification *)notification
{
    if (!self.inputCell.editing)
    {
        return;
    }
    
    [[CompletionsViewController instance] dismissAnimated:YES];
    [self completionsPopupDismissed];

    Completion *completion = notification.object;
    [_pythonEditorView insertString:completion.completion];
    _pythonEditorView.selectedRange = NSMakeRange(_pythonEditorView.selectedRange.location + completion.completion.length, 0);
}

- (void)completionsPopupDismissed:(NSNotification *)notification
{
    if (!self.inputCell.editing)
    {
        return;
    }
    [self completionsPopupDismissed];
}

- (void)fontSettingsDidChange:(NSNotification *)notification
{
    _inLabel.font = [Settings instance].codeFont.regularFont;
    _streamOutputView.font = [Settings instance].codeFont.regularFont;
    _markdownSourceEditorView.font = [Settings instance].codeFont.regularFont;
    _pythonEditorView.font = [Settings instance].codeFont.regularFont;
    _placeholderTextView.font = [Settings instance].codeFont.italicFont;
}

#pragma mark - KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if (object == self.inputCell)
    {
        //        NSLog(@"value did change on %@ for key path: %@, %@ -> %@", self.inputCell, keyPath, change[NSKeyValueChangeOldKey], change[NSKeyValueChangeNewKey]);
        
        if ([keyPath isEqualToString:@"focused"])
        {
            if (!self.inputCell.focused && self.inputCell.editing)
            {
                [self endEditing];
            }
            _pythonEditorView.layer.borderColor = self.inputCell.focused ? [FocusedBorderColor CGColor] : [BorderColor CGColor];
            _markdownSourceEditorView.layer.borderColor = self.inputCell.focused ? [FocusedBorderColor CGColor] : [BorderColor CGColor];
            return;
        }
        
        if ([keyPath isEqualToString:@"editing"])
        {
            [self updateInputViews];
            if (self.inputCell.editing)
            {
                if (!_suppressStartEditingOnKVO)
                {
                    [self startEditing];
                }
            }
            else
            {
                if (!_suppressEndEditingOnKVO)
                {
                    [self endEditing];
                }
                [self hideInfoButton];
            }
            return;
        }
        
        if ([keyPath isEqualToString:@"executing"])
        {
            [self showStar:self.inputCell.executing];
            return;
        }
        
        if ([keyPath isEqualToString:@"type"])
        {
            _inLabel.hidden = self.inputCell.isMarkdown;
            _streamOutputHeightConstraint.constant = 0;
            [self updateInputViews];
            if (self.inputCell.editing)
            {
                if (!_suppressStartEditingOnKVO)
                {
                    [self startEditing];
                }
            }
            return;
        }
    }
}

#pragma mark - NSObject(UINibLoadingAdditions)

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    self.backgroundColor = [UIColor whiteColor];
    //    self.layer.borderWidth = 1;
    //    self.layer.borderColor = [[UIColor greenColor] CGColor];
    
    _infoButton.layer.cornerRadius = 11;
//    _infoButton.layer.borderColor = _infoButton.tintColor.CGColor;
//    _infoButton.layer.borderWidth = 1;
    _infoButton.layer.shadowColor = [UIColor blackColor].CGColor;
    _infoButton.layer.shadowOpacity = 0.2;
    _infoButton.layer.shadowRadius = 5;
    _infoButton.layer.shadowOffset = CGSizeMake(0, 5);
    
    _inLabel.font = [Settings instance].codeFont.regularFont;
    
    _streamOutputView.font = [Settings instance].codeFont.regularFont;
    _streamOutputView.backgroundColor = [UIColor clearColor];
    _streamOutputView.textContainerInset = EditorTextContainerInsets;
    _streamOutputView.editable = NO;
    
    _markdownView = [[UIImageView alloc] initWithFrame:_editorPlaceholderView.bounds];
    _markdownView.backgroundColor = [UIColor whiteColor];
    _markdownView.contentMode = UIViewContentModeTop;
    _markdownView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    _markdownView.userInteractionEnabled = YES;
    _markdownView.frame = _editorPlaceholderView.bounds;
    [_editorPlaceholderView addSubview:_markdownView];
    
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(markdownTapped:)];
    [_markdownView addGestureRecognizer:tapRecognizer];
    
    _markdownSourceEditorView = [MarkdownEditorView newInstanceWithFrame:_editorPlaceholderView.bounds];
    _markdownSourceEditorView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    _markdownSourceEditorView.backgroundColor = LightGrayBackground;
    _markdownSourceEditorView.layer.borderWidth = 1;
    _markdownSourceEditorView.layer.borderColor = [BorderColor CGColor];
    _markdownSourceEditorView.textContainerInset = EditorTextContainerInsets;
    _markdownSourceEditorView.font = [Settings instance].codeFont.regularFont;
    _markdownSourceEditorView.editorDelegate = self;
    [_editorPlaceholderView addSubview:_markdownSourceEditorView];
    
    _pythonEditorView = [PythonEditorView newInstanceWithFrame:_editorPlaceholderView.bounds];
    _pythonEditorView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    _pythonEditorView.backgroundColor = LightGrayBackground;
    _pythonEditorView.layer.borderWidth = 1;
    _pythonEditorView.layer.borderColor = [BorderColor CGColor];
    _pythonEditorView.textContainerInset = EditorTextContainerInsets;
    _pythonEditorView.font = [Settings instance].codeFont.regularFont;
    _pythonEditorView.editorDelegate = self;
    [_editorPlaceholderView addSubview:_pythonEditorView];
    
    _placeholderTextView = [[UITextView alloc] initWithFrame:_editorPlaceholderView.bounds];
    _placeholderTextView.translatesAutoresizingMaskIntoConstraints = NO;
    _placeholderTextView.backgroundColor = [UIColor clearColor];
    _placeholderTextView.font = [Settings instance].codeFont.italicFont;
    _placeholderTextView.textContainerInset = EditorTextContainerInsets;
    _placeholderTextView.textColor = [UIColor lightGrayColor];
    _placeholderTextView.userInteractionEnabled = NO;
    [_editorPlaceholderView addSubview:_placeholderTextView];
    [_editorPlaceholderView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(0)-[_placeholderTextView]-(0)-|"
                                                                                   options:0
                                                                                   metrics:nil
                                                                                     views:NSDictionaryOfVariableBindings(_placeholderTextView)]];
    CGFloat height = ceilf([Settings instance].codeFont.regularFont.lineHeight) + EditorTextContainerInsets.top + EditorTextContainerInsets.bottom;;
    [_editorPlaceholderView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[_placeholderTextView(height)]"
                                                                                   options:0
                                                                                   metrics:@{ @"height" : @(height) }
                                                                                     views:NSDictionaryOfVariableBindings(_placeholderTextView)]];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(completionsPopupDidSelectCompletion:)
                                                 name:CompletionsPopupDidSelectCompletionNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(completionsPopupDismissed:)
                                                 name:CompletionsPopupDidDismissNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(fontSettingsDidChange:)
                                                 name:FontSettingsDidChangeNotification
                                               object:nil];
}

#pragma mark - EditorViewDelegate

- (BOOL)editorViewWantsBecomeFirstResponder:(EditorView *)editorView
{
    BOOL acceptFirstResponder = self.inputCell.focused || self.inputCell.source.length == 0;
    if (!self.inputCell.focused)
    {
        self.inputCell.notebook.focusedInputCell = self.inputCell;
    }
    if (acceptFirstResponder && !self.inputCell.editing)
    {
        _suppressStartEditingOnKVO = YES;
        self.inputCell.editing = YES;
        _suppressStartEditingOnKVO = NO;
    }
    return acceptFirstResponder;
}

- (BOOL)editorViewWantsResignFirstResponder:(EditorView *)editorView
{
    if (self.inputCell.editing)
    {
        if (self.inputCell.preventEndEditingOnResignFirstResponder)
        {
            self.inputCell.preventEndEditingOnResignFirstResponder = NO;
            return YES;
        }
        _suppressEndEditingOnKVO = YES;
        self.inputCell.editing = NO;
        _suppressEndEditingOnKVO = NO;
    }
    return YES;
}

- (void)editorViewTextDidChange:(EditorView *)editorView
{
    BOOL wasEmpty = self.inputCell.source.length == 0;
    if (editorView == _pythonEditorView)
    {
        self.inputCell.source = _pythonEditorView.text;
        self.inputCell.selectedRange = _pythonEditorView.selectedRange;
        
        [self computeCaretLocation];
        if (_showCompletionsPopup)
        {
            [self showCompletionsPopup];
        }
        else
        {
            [self moveInfoButtonToCaretLocation];
            [self showInfoButton];
        }
    }
    else if (editorView == _markdownSourceEditorView)
    {
        self.inputCell.source = _markdownSourceEditorView.text;
    }
    BOOL sourceEmptyChanged = NO;
    if (wasEmpty && self.inputCell.source.length > 0)
    {
        sourceEmptyChanged = YES;
        [UIView animateWithDuration:kFadeDuration
                         animations:^{
                             _placeholderTextView.alpha = 0;
                         } completion:^(BOOL finished) {
                             if (!finished)
                             {
                                 return;
                             }
                             _markdownSourceEditorView.font = [Settings instance].codeFont.regularFont;
                             [_editorPlaceholderView sendSubviewToBack:_placeholderTextView];
                         }];
    }
    else if (!wasEmpty && self.inputCell.source.length == 0)
    {
        sourceEmptyChanged = YES;
        [_editorPlaceholderView bringSubviewToFront:_placeholderTextView];
        [UIView animateWithDuration:kFadeDuration
                         animations:^{
                             _placeholderTextView.alpha = 1;
                         }];
    }
    if (sourceEmptyChanged)
    {
        [[NSNotificationCenter defaultCenter] postNotificationName:InputCellSourceEmptyDidChangeNotification object:self.inputCell];
    }
}

- (void)editorViewHeightDidChange:(EditorView *)editorView
{
    if (editorView.superview == _editorPlaceholderView)
    {
        CGFloat layoutWidth = CGRectGetWidth(self.bounds);
        _editorPlaceholderHeightConstraint.constant = [InputCellView preferredCodeHeightForCell:self.inputCell
                                                                                  sourceEditing:self.inputCell.editing
                                                                                    layoutWidth:layoutWidth];
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:NotebookCellHeightDidChangeNotification object:self.inputCell];
}

#pragma mark - PythonEditorViewDelegate

- (void)pythonEditorViewDidRequestCommand:(PythonEditorCommand)command
{
    switch (command)
    {
        case PythonEditorCommandRun:
            [self execute];
            break;
        default:
            break;
    }
}

@end
