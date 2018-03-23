//
//  PythonEditorView.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonEditorView.h"
#import "PythonInputAccessoryView.h"
#import "PythonTextStorage.h"

// for code completion, see InteractiveShell complete

@implementation PythonEditorView

#pragma mark - Statics

+ (PythonEditorView *)newInstanceWithFrame:(CGRect)frame
{
    PythonTextStorage *textStorage = [PythonTextStorage new];
    NSLayoutManager *layoutManager = [NSLayoutManager new];
    [textStorage addLayoutManager:layoutManager];
    NSTextContainer *textContainer = [[NSTextContainer alloc] initWithSize:frame.size];
    [layoutManager addTextContainer:textContainer];
    
    return [[PythonEditorView alloc] initWithFrame:frame textContainer:textContainer];
}

#pragma mark - PythonEditorView

- (id)initWithFrame:(CGRect)frame textContainer:(NSTextContainer *)textContainer
{
    if (self = [super initWithFrame:frame textContainer:textContainer])
    {
        self.autocapitalizationType = UITextAutocapitalizationTypeNone;
        self.autocorrectionType = UITextAutocorrectionTypeNo;
        self.spellCheckingType = UITextSpellCheckingTypeNo;
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(pythonTrackpadDidMoveCursor:)
                                                     name:PythonTrackpadDidMoveNotification
                                                   object:nil];
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)pythonTrackpadDidMoveCursor:(NSNotification *)notification
{
    [self trackpadDidMoveCursor:notification];
}

#pragma mark - UIResponder

- (BOOL)becomeFirstResponder
{
    PythonInputAccessoryView *accessoryView = [PythonInputAccessoryView instance];
    [accessoryView linkWithEditor:self];
    [accessoryView updateKeyboardLayoutForInterfaceOrientation:[UIApplication sharedApplication].statusBarOrientation];
    self.inputAccessoryView = accessoryView;
    
    return [super becomeFirstResponder];
}

#pragma mark - PythonInputAccessoryDelegate

- (void)tab:(id)sender
{
    [self insertString:@"\t"];
}

- (void)underscore:(id)sender
{
    [self insertString:@"_"];
}

- (void)hashSign:(id)sender
{
    [self insertString:@"#"];
}

- (void)lessThan:(id)sender
{
    [self insertString:@"<"];
}

- (void)greaterThan:(id)sender
{
    [self insertString:@">"];
}

- (void)openingBrace:(id)sender
{
    [self insertString:@"{"];
}

- (void)closingBrace:(id)sender
{
    [self insertString:@"}"];
}

- (void)openingBracket:(id)sender
{
    [self insertString:@"["];
}

- (void)closingBracket:(id)sender
{
    [self insertString:@"]"];
}

- (void)openingParen:(id)sender
{
    [self insertString:@"("];
}

- (void)closingParen:(id)sender
{
    [self insertString:@")"];
}

- (void)singleQuote:(id)sender
{
    [self insertString:@"'"];
}

- (void)colon:(id)sender
{
    [self insertString:@":"];
}

- (void)run:(id)sender
{
    if ([self.editorDelegate respondsToSelector:@selector(pythonEditorViewDidRequestCommand:)])
    {
        [((id<PythonEditorViewDelegate>)self.editorDelegate) pythonEditorViewDidRequestCommand:PythonEditorCommandRun];
    }
}

- (void)one:(id)sender
{
    [self insertString:@"1"];
}

- (void)two:(id)sender
{
    [self insertString:@"2"];
}

- (void)three:(id)sender
{
    [self insertString:@"3"];
}

- (void)four:(id)sender
{
    [self insertString:@"4"];
}

- (void)five:(id)sender
{
    [self insertString:@"5"];
}

- (void)six:(id)sender
{
    [self insertString:@"6"];
}

- (void)seven:(id)sender
{
    [self insertString:@"7"];
}

- (void)eight:(id)sender
{
    [self insertString:@"8"];
}

- (void)nine:(id)sender
{
    [self insertString:@"9"];
}

- (void)zero:(id)sender
{
    [self insertString:@"0"];
}

- (void)plus:(id)sender
{
    [self insertString:@"+"];
}

- (void)minus:(id)sender
{
    [self insertString:@"-"];
}

- (void)asterisk:(id)sender
{
    [self insertString:@"*"];
}

- (void)slash:(id)sender
{
    [self insertString:@"/"];
}

- (void)assign:(id)sender
{
    [self insertString:@"="];
}

@end
