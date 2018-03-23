//
//  TabView.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TabView.h"
#import "TabsViewController.h"
#import "../../../Common/Utilities/UIImage+Resizing.h"

static const CGFloat kCloseButtonWidth = 32;

@implementation TabView
{
    UIButton *_closeButton;
    NSLayoutConstraint *_closeButtonWidthConstraint;
    UIActivityIndicatorView *_spinner;
}

#pragma mark - Properties

- (void)setActive:(BOOL)active
{
    _active = active;
    if (!_active)
    {
        [_spinner stopAnimating];
    }
    _closeButton.hidden = !active || [_spinner isAnimating];
    self.backgroundColor = [UIColor colorWithWhite:active ? 0.98 : 0.94 alpha:1.0];
    _closeButtonWidthConstraint.constant = active ? kCloseButtonWidth : 0;
    [self layoutIfNeeded];
}

#pragma mark - UIView

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
    {
        self.translatesAutoresizingMaskIntoConstraints = NO;
        self.backgroundColor = [UIColor colorWithWhite:0.98 alpha:1.0];
        
        _label = [[UILabel alloc] initWithFrame:CGRectZero];
        _label.translatesAutoresizingMaskIntoConstraints = NO;
        _label.font = [UIFont systemFontOfSize:13];
        _label.lineBreakMode = NSLineBreakByTruncatingTail;
        _label.minimumScaleFactor = 0.5;
        [self addSubview:_label];
        
        _closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _closeButton.translatesAutoresizingMaskIntoConstraints = NO;
        _closeButton.contentMode = UIViewContentModeCenter;
        [_closeButton setImage:[[UIImage imageNamed:@"Close"] resizedImageWithWidth:16]
                      forState:UIControlStateNormal];
        [_closeButton addTarget:self
                         action:@selector(close:)
               forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:_closeButton];
        
        _closeButtonWidthConstraint = [NSLayoutConstraint constraintWithItem:_closeButton
                                                                   attribute:NSLayoutAttributeWidth
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:nil
                                                                   attribute:NSLayoutAttributeNotAnAttribute
                                                                  multiplier:1
                                                                    constant:32];
        [self addConstraint:_closeButtonWidthConstraint];
        
        [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(6)-[_label]-(0)-[_closeButton]-(0)-|"
                                                                     options:0
                                                                     metrics:nil
                                                                       views:NSDictionaryOfVariableBindings(_label, _closeButton)]];
        [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(3)-[_label]|"
                                                                     options:0
                                                                     metrics:nil
                                                                       views:NSDictionaryOfVariableBindings(_label)]];
        [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[_closeButton]-(0)-|"
                                                                     options:0
                                                                     metrics:nil
                                                                       views:NSDictionaryOfVariableBindings(_closeButton)]];
        
        _spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        _spinner.translatesAutoresizingMaskIntoConstraints = noErr;
        _spinner.hidesWhenStopped = YES;
        _spinner.transform = CGAffineTransformMakeScale(0.7, 0.7);
        [self addSubview:_spinner];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:_spinner
                                                         attribute:NSLayoutAttributeCenterX
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:_closeButton
                                                         attribute:NSLayoutAttributeCenterX
                                                        multiplier:1
                                                          constant:0]];
        [self addConstraint:[NSLayoutConstraint constraintWithItem:_spinner
                                                         attribute:NSLayoutAttributeCenterY
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:_closeButton
                                                         attribute:NSLayoutAttributeCenterY
                                                        multiplier:1
                                                          constant:0]];
        
        UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapped:)];
        [self addGestureRecognizer:tapRecognizer];
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    [super drawRect:rect];
    
    CGFloat lineWidth = 1 / [UIScreen mainScreen].scale;
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetStrokeColorWithColor(context, [UIColor lightGrayColor].CGColor);
    CGContextSetLineWidth(context, lineWidth);
    
    CGFloat h = CGRectGetHeight(self.bounds);
    CGFloat w = CGRectGetWidth(self.bounds);
//    CGContextMoveToPoint(context, lineWidth / 2, h);
//    CGContextAddLineToPoint(context, lineWidth / 2, lineWidth / 2);
    CGContextMoveToPoint(context, lineWidth / 2, lineWidth / 2);
    CGContextAddLineToPoint(context, w - lineWidth / 2, lineWidth / 2);
    CGContextAddLineToPoint(context, w - lineWidth / 2, h);
    
    CGContextStrokePath(context);
}

#pragma mark - TabView

- (void)startSpinner
{
    if (!_active)
    {
        return;
    }
    _closeButton.hidden = YES;
    [_spinner startAnimating];
}

- (void)stopSpinner
{
    if (!_active)
    {
        return;
    }
    [_spinner stopAnimating];
    _closeButton.hidden = NO;
}

- (void)close:(id)sender
{
    [_tabsController closeTab:self];
}

- (void)tapped:(UIGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        [_tabsController activateTab:self];
    }
}

@end
