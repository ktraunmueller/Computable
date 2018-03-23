//
//  PythonInputAccessoryView.m
//  Computable
//
//  Created by Karl Traunmüller on 27.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonInputAccessoryView.h"
#import "PythonEditorView.h"
#import "DelegatingButton.h"
#import <objc/runtime.h>

NSString *const PythonTrackpadDidMoveNotification = @"PythonTrackpadDidMoveNotification";

static const CGFloat kHorizontalMargin = 7;
static const CGFloat kVerticalMargin = 7;
static const CGFloat kHorizontalPadding = 14;
static const CGFloat kVerticalPadding = 9;

//static const CGFloat kButtonWidth = 79;
static const CGFloat kButtonHeight = 50; // 74pt for regular keyboard buttons
static const CGFloat kCornerRadius = 7;
static const CGFloat kButtonFontSize = 22;

static const NSUInteger kNumberOfButtonsPerRow = 15;

static NSString *const kUserDidDragKeyboardDefaultsKey = @"DidDragKeyboard";

@interface PythonInputAccessoryView () <UIScrollViewDelegate>

@end

@implementation PythonInputAccessoryView
{
    PythonEditorView *_editor;
    UIScrollView *_scrollView;
    UIView *_buttonsContainerView;
    UIView *_trackpadContainerView;
    UIView *_trackpadView;
    CGPoint _lastTrackingPoint;
    UILabel *_scrollHintLabel;
    UIButton *_runButton;
    NSMutableArray *_buttonsInRow1;
    NSMutableArray *_buttonsInRow2;
    NSArray *_buttons;
    UIInterfaceOrientation _interfaceOrientation;
}

#pragma mark - Statics

+ (PythonInputAccessoryView *)instance
{
    static PythonInputAccessoryView *instance = nil;
    if (instance == nil)
    {
        CGRect screenBounds = [UIScreen mainScreen].bounds;
        CGRect frame = CGRectMake(0, 0, CGRectGetWidth(screenBounds), kVerticalMargin + 2 * kButtonHeight + kVerticalPadding);
        instance = [[PythonInputAccessoryView alloc] initWithFrame:frame];
    }
    return instance;
}

#pragma mark - PythonInputAccessoryView

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame inputViewStyle:UIInputViewStyleKeyboard])
    {
//        self.backgroundColor = [UIColor colorWithRed:220/255.0 green:223/255.0 blue:228/255.0 alpha:1.0];
        
        _scrollView = [[UIScrollView alloc] initWithFrame:self.bounds];
        _scrollView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        _scrollView.showsHorizontalScrollIndicator = NO;
        _scrollView.contentSize = CGSizeMake(2 * CGRectGetWidth(self.bounds), CGRectGetHeight(self.bounds));
        _scrollView.pagingEnabled = YES;
        _scrollView.delegate = self;
        [self addSubview:_scrollView];
        _scrollView.backgroundColor = [UIColor clearColor];
        
        _buttonsContainerView = [[UIView alloc] initWithFrame:self.bounds];
        _buttonsContainerView.translatesAutoresizingMaskIntoConstraints = NO;
        [_scrollView addSubview:_buttonsContainerView];
        _buttonsContainerView.backgroundColor = [UIColor clearColor];
        
        _trackpadContainerView = [[UIView alloc] initWithFrame:self.bounds];
        _trackpadContainerView.translatesAutoresizingMaskIntoConstraints = NO;
        [_scrollView addSubview:_trackpadContainerView];
        _trackpadContainerView.backgroundColor = [UIColor clearColor];
        
        _trackpadView = [[UIView alloc] initWithFrame:CGRectZero];
        _trackpadView.translatesAutoresizingMaskIntoConstraints = NO;
        _trackpadView.backgroundColor = [[UIColor whiteColor] colorWithAlphaComponent:0.8];
        _trackpadView.layer.cornerRadius = 10;
        _trackpadView.layer.shadowColor = [UIColor darkGrayColor].CGColor;
        _trackpadView.layer.shadowRadius = 0;
        _trackpadView.layer.shadowOpacity = 0.6;
        _trackpadView.layer.shadowOffset = CGSizeMake(0, -1);
        [_trackpadContainerView addSubview:_trackpadView];
        
        UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panning:)];
        [_trackpadView addGestureRecognizer:panRecognizer];
        
        _scrollHintLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _scrollHintLabel.translatesAutoresizingMaskIntoConstraints = NO;
        _scrollHintLabel.font = [UIFont systemFontOfSize:60];
        _scrollHintLabel.textColor = [UIColor grayColor];
        _scrollHintLabel.textAlignment = NSTextAlignmentCenter;
        _scrollHintLabel.backgroundColor = [UIColor clearColor];
        _scrollHintLabel.text = @"⇢";
        _scrollHintLabel.alpha = [[NSUserDefaults standardUserDefaults] boolForKey:kUserDidDragKeyboardDefaultsKey] ? 0 : 1;
        [_scrollView addSubview:_scrollHintLabel];
        [_scrollHintLabel sizeToFit];
        
        _buttonsInRow1 = [NSMutableArray array];
        _buttonsInRow2 = [NSMutableArray array];
        
        // row 1
        DelegatingButton *button = [self buttonWithTitle:@"⇥" action:@selector(tab:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"#" action:@selector(hashSign:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@":" action:@selector(colon:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"_" action:@selector(underscore:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"'" action:@selector(singleQuote:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"<" action:@selector(lessThan:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@">" action:@selector(greaterThan:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"{" action:@selector(openingBrace:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"}" action:@selector(closingBrace:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"[" action:@selector(openingBracket:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"]" action:@selector(closingBracket:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"(" action:@selector(openingParen:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@")" action:@selector(closingParen:)];
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"➠" action:@selector(run:)];
        button.titleLabel.font = [button.titleLabel.font fontWithSize:54];
        [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [button setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
        button.normalColor = [UIColor colorWithRed:186/255.0 green:189/255.0 blue:193/255.0 alpha:1.0];
        button.highlightedColor = [UIColor whiteColor];
        button.backgroundColor = button.normalColor;
        _runButton = button;
        [_buttonsInRow1 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        // row 2
        button = [self buttonWithTitle:@"1" action:@selector(one:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"2" action:@selector(two:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"3" action:@selector(three:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"4" action:@selector(four:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"5" action:@selector(five:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"6" action:@selector(six:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"7" action:@selector(seven:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"8" action:@selector(eight:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"9" action:@selector(nine:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"0" action:@selector(zero:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"+" action:@selector(plus:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"-" action:@selector(minus:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"*" action:@selector(asterisk:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"/" action:@selector(slash:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        button = [self buttonWithTitle:@"=" action:@selector(assign:)];
        [_buttonsInRow2 addObject:button];
        [_buttonsContainerView addSubview:button];
        
        _buttons = [_buttonsInRow1 arrayByAddingObjectsFromArray:_buttonsInRow2];
        
        _interfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
        [self setNeedsUpdateConstraints];
    }
    return self;
}

- (DelegatingButton *)buttonWithTitle:(NSString *)title action:(SEL)action
{
    DelegatingButton *button = [DelegatingButton buttonWithType:UIButtonTypeRoundedRect];
    button.translatesAutoresizingMaskIntoConstraints = NO;
    
    button.backgroundColor = [UIColor whiteColor];
    button.adjustsImageWhenHighlighted = NO;
    button.showsTouchWhenHighlighted = NO;
    button.layer.cornerRadius = kCornerRadius;
    button.layer.shadowColor = [UIColor darkGrayColor].CGColor;
    button.layer.shadowRadius = 0;
    button.layer.shadowOpacity = 0.6;
    button.layer.shadowOffset = CGSizeMake(0, 1);
    
    [button setTitle:title forState:UIControlStateNormal];
    button.titleLabel.font = [UIFont systemFontOfSize:kButtonFontSize];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
    
    [button addTarget:self action:@selector(dispatchTouchDown:) forControlEvents:UIControlEventTouchDown];
    button.action = action;
    
    return button;
}

- (void)dispatchTouchDown:(id)sender
{
    [[UIDevice currentDevice] playInputClick];
    
    DelegatingButton *button = sender;
    IMP imp = [_editor methodForSelector:button.action];
    void (*func)(id, SEL, id) = (void *)imp;
    func(_editor, button.action, button);
}

- (void)linkWithEditor:(PythonEditorView *)editor
{
    _editor = editor;
}

- (void)showScrollHint
{
    if (_scrollHintLabel.alpha < 1 || [[NSUserDefaults standardUserDefaults] boolForKey:kUserDidDragKeyboardDefaultsKey])
    {
        return;
    }
    _scrollHintLabel.layer.transform = CATransform3DMakeScale(0.01, 0.01, 1);
    [UIView animateWithDuration:0.4
                          delay:0
                        options:UIViewAnimationOptionCurveEaseInOut | UIViewAnimationOptionAllowUserInteraction
                     animations:^{
                         _scrollView.contentOffset = CGPointMake(100, 0);
                     } completion:^(BOOL finished) {
                         if (!finished)
                         {
                             return;
                         }
                         [UIView animateWithDuration:0.8
                                               delay:0.2
                              usingSpringWithDamping:0.2
                               initialSpringVelocity:0
                                             options:0
                                          animations:^{
                                              _scrollHintLabel.layer.transform = CATransform3DIdentity;
                                          } completion:nil];
                         [UIView animateWithDuration:0.4
                                               delay:2
                                             options:UIViewAnimationOptionCurveEaseInOut | UIViewAnimationOptionAllowUserInteraction
                                          animations:^{
                                              _scrollView.contentOffset = CGPointZero;
                                          } completion:nil];
                     }];
}

- (void)updateKeyboardLayoutForInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (_interfaceOrientation == interfaceOrientation)
    {
        return;
    }
    _interfaceOrientation = interfaceOrientation;
    _scrollView.contentOffset = CGPointZero;
    [self setNeedsUpdateConstraints];
}

- (void)panning:(UIPanGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        _lastTrackingPoint = CGPointZero;
        return;
    }
    CGPoint point = [recognizer translationInView:_trackpadView];
    CGPoint offset = CGPointMake(point.x - _lastTrackingPoint.x, point.y - _lastTrackingPoint.y);
    [[NSNotificationCenter defaultCenter] postNotificationName:PythonTrackpadDidMoveNotification
                                                        object:[NSValue valueWithCGPoint:offset]];
    _lastTrackingPoint = point;
}

#pragma mark - UIView

- (void)updateConstraints
{
    [_scrollView removeConstraints:_scrollView.constraints];
    [_buttonsContainerView removeConstraints:_buttonsContainerView.constraints];
    [_trackpadContainerView removeConstraints:_trackpadContainerView.constraints];
    
    CGRect screenBounds = [UIScreen mainScreen].bounds;
    CGFloat screenWidth = CGRectGetWidth(screenBounds);
    CGFloat screenHeight = CGRectGetHeight(screenBounds);
    CGFloat availableWidth = UIInterfaceOrientationIsLandscape(_interfaceOrientation) ? fmax(screenWidth, screenHeight) : fmin(screenWidth, screenHeight);
    
    CGFloat pageSize = availableWidth;
    CGFloat height = CGRectGetHeight(self.bounds);
    _scrollView.contentSize = CGSizeMake(2 * pageSize, height);
    [_scrollView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(0)-[_buttonsContainerView(pageSize)]-(0)-[_trackpadContainerView(pageSize)]-(0)-|"
                                                                        options:0
                                                                        metrics:@{ @"pageSize" : @(pageSize) }
                                                                          views:NSDictionaryOfVariableBindings(_buttonsContainerView, _trackpadContainerView)]];
    [_scrollView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[_buttonsContainerView(height)]-(0)-|"
                                                                        options:0
                                                                        metrics:@{ @"height" : @(height) }
                                                                          views:NSDictionaryOfVariableBindings(_buttonsContainerView)]];
    [_scrollView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[_trackpadContainerView(height)]-(0)-|"
                                                                        options:0
                                                                        metrics:@{ @"height" : @(height) }
                                                                          views:NSDictionaryOfVariableBindings(_trackpadContainerView)]];
    _scrollHintLabel.layer.transform = CATransform3DIdentity;
    CGSize hintSize = _scrollHintLabel.frame.size;
    [_scrollView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(offset)-[_scrollHintLabel(100)]"
                                                                        options:0
                                                                        metrics:@{ @"offset" : @(pageSize) }
                                                                          views:NSDictionaryOfVariableBindings(_scrollHintLabel)]];
    [_scrollView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(voffset)-[_scrollHintLabel(height)]-(voffset)-|"
                                                                        options:0
                                                                        metrics:@{ @"height" : @(hintSize.height),
                                                                                   @"voffset" : @((height - hintSize.height) / 2) }
                                                                          views:NSDictionaryOfVariableBindings(_scrollHintLabel)]];
    
    CGFloat panelWidth = 400;
    CGFloat offset = (availableWidth - panelWidth) / 2;
    [_trackpadContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(offset)-[_trackpadView(width)]-(offset)-|"
                                                                                         options:0
                                                                                         metrics:@{ @"offset" : @(offset),
                                                                                                    @"width" : @(panelWidth) }
                                                                                           views:NSDictionaryOfVariableBindings(_trackpadView)]];
    offset = 8;
    CGFloat panelHeight = height - offset - 4;
    [_trackpadContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(offset)-[_trackpadView(height)]-(4)-|"
                                                                                         options:0
                                                                                         metrics:@{ @"offset" : @(offset),
                                                                                                    @"height" : @(panelHeight) }
                                                                                           views:NSDictionaryOfVariableBindings(_trackpadView)]];
    
    CGFloat hPadding = UIInterfaceOrientationIsLandscape(_interfaceOrientation) ? kHorizontalPadding : kHorizontalPadding * 0.7;
    CGFloat buttonWidth = roundf((availableWidth - 2 * kHorizontalMargin - (kNumberOfButtonsPerRow - 1) * hPadding) / kNumberOfButtonsPerRow);
    CGFloat buttonWidthDouble = 2 * buttonWidth + hPadding;
    
    UIButton *previousButton = nil;
    for (UIButton *button in _buttonsInRow1)
    {
        if (previousButton == nil)
        {
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(margin)-[button(width)]"
                                                                                          options:0
                                                                                          metrics:@{ @"width" : @(buttonWidth),
                                                                                                     @"margin" : @(kHorizontalMargin) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(margin)-[button(height)]"
                                                                                          options:0
                                                                                          metrics:@{ @"height" : @(kButtonHeight),
                                                                                                     @"margin": @(kVerticalMargin) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
        }
        else
        {
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:[previousButton]-(padding@900)-[button(width)]"
                                                                                          options:0
                                                                                          metrics:@{ @"width" : @(button == _runButton ? buttonWidthDouble : buttonWidth),
                                                                                                     @"padding" : @(hPadding) }
                                                                                            views:NSDictionaryOfVariableBindings(previousButton, button)]];
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(margin)-[button(height)]"
                                                                                          options:0
                                                                                          metrics:@{ @"height" : @(kButtonHeight),
                                                                                                     @"margin": @(kVerticalMargin) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
        }
        previousButton = button;
    }
    
    previousButton = nil;
    for (UIButton *button in _buttonsInRow2)
    {
        if (previousButton == nil)
        {
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(margin)-[button(width)]"
                                                                                          options:0
                                                                                          metrics:@{ @"width" : @(buttonWidth),
                                                                                                     @"margin" : @(kHorizontalMargin) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(margin)-[button(height)]"
                                                                                          options:0
                                                                                          metrics:@{ @"height" : @(kButtonHeight),
                                                                                                     @"margin": @(kVerticalMargin + kButtonHeight + kVerticalPadding - 1) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
        }
        else
        {
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:[previousButton]-(padding@900)-[button(width)]"
                                                                                          options:0
                                                                                          metrics:@{ @"width" : @(buttonWidth),
                                                                                                     @"padding" : @(hPadding) }
                                                                                            views:NSDictionaryOfVariableBindings(previousButton, button)]];
            [_buttonsContainerView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(margin)-[button(height)]"
                                                                                          options:0
                                                                                          metrics:@{ @"height" : @(kButtonHeight),
                                                                                                     @"margin": @(kVerticalMargin + kButtonHeight + kVerticalPadding - 1) }
                                                                                            views:NSDictionaryOfVariableBindings(button)]];
        }
        previousButton = button;
    }
    
    [super updateConstraints];
}

#pragma mark - UIScrollViewDelegate

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    if (_scrollView.contentOffset.x > 150 && _scrollHintLabel.alpha == 1 && ![userDefaults boolForKey:kUserDidDragKeyboardDefaultsKey])
    {
        [UIView animateWithDuration:0.5
                         animations:^{
                             _scrollHintLabel.alpha = 0;
                         }];
        [userDefaults setBool:YES forKey:kUserDidDragKeyboardDefaultsKey];
        [userDefaults synchronize];
    }
}

#pragma mark - UIInputViewAudioFeedback

- (BOOL)enableInputClicksWhenVisible
{
    return YES;
}

@end
