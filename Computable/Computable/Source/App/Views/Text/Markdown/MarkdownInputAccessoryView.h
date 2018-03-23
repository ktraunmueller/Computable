//
//  MarkdownInputAccessoryView.h
//  Computable
//
//  Created by Karl Traunmüller on 27.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const MarkdownTrackpadDidMoveNotification;

@class MarkdownEditorView;

@interface MarkdownInputAccessoryView : UIInputView <UIInputViewAudioFeedback>

+ (MarkdownInputAccessoryView *)instance;

- (void)linkWithEditor:(MarkdownEditorView *)editor;
- (void)showScrollHint;
- (void)updateKeyboardLayoutForInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;

@end

@protocol MarkdownInputAccessoryDelegate <NSObject>

- (void)tab:(id)sender;
- (void)underscore:(id)sender;
- (void)hashSign:(id)sender;
- (void)dollar:(id)sender;
- (void)lessThan:(id)sender;
- (void)greaterThan:(id)sender;
- (void)openingBrace:(id)sender;
- (void)closingBrace:(id)sender;
- (void)openingBracket:(id)sender;
- (void)closingBracket:(id)sender;
- (void)openingParen:(id)sender;
- (void)closingParen:(id)sender;
- (void)singleQuote:(id)sender;
- (void)doubleQuote:(id)sender;
- (void)backtick:(id)sender;
- (void)colon:(id)sender;
- (void)one:(id)sender;
- (void)two:(id)sender;
- (void)three:(id)sender;
- (void)four:(id)sender;
- (void)five:(id)sender;
- (void)six:(id)sender;
- (void)seven:(id)sender;
- (void)eight:(id)sender;
- (void)nine:(id)sender;
- (void)zero:(id)sender;
- (void)plus:(id)sender;
- (void)minus:(id)sender;
- (void)asterisk:(id)sender;
- (void)slash:(id)sender;
- (void)backslash:(id)sender;
- (void)assign:(id)sender;

@end
