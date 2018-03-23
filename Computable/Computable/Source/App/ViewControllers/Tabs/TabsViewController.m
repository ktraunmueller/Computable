//
//  TabsViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TabsViewController.h"
#import "TabView.h"
#import "../../../Common/Utilities/UIColor+Random.h"
#import "../../../Common/Utilities/UIViewController+Containment.h"

NSString *const SelectedTabDidChangeNotification = @"SelectedTabDidChangeNotification";

static const CGFloat kTabHeight = 32;
static const CGFloat kTabWidth = 160;

@implementation TabsViewController
{
    NSMutableArray *_tabs;
    UIView *_tabsView;
    UIView *_leftHairlineView;
    NSLayoutConstraint *_leftHairlineViewWidthConstraint;
    UIView *_rightHairlineView;
    NSLayoutConstraint *_rightHairlineViewLeadingConstraint;
    UIView *_contentView;
    NSLayoutConstraint *_bottomConstraint;
}

#pragma mark - Statics

+ (TabsViewController *)newInstance
{
    return [[TabsViewController alloc] initWithNibName:nil bundle:nil];
}

#pragma mark - TabsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        _tabs = [NSMutableArray array];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillShow:)
                                                     name:UIKeyboardWillShowNotification
                                                   object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillHide:)
                                                     name:UIKeyboardWillHideNotification
                                                   object:nil];
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (TabView *)addTabController:(UIViewController<TabsViewControllerChild> *)tabController
{
    if (tabController == nil)
    {
        return nil;
    }
    
    CGRect frame = CGRectMake(0, 0, kTabWidth, kTabHeight);
    TabView *tabView = [[TabView alloc] initWithFrame:frame];
    tabView.tabsController = self;
    tabView.contentController = tabController;
    [_tabs addObject:tabView];
    [_tabsView addSubview:tabView];
    
    [self updateTabConstraints];
    [self activateTab:tabView];
    
    return tabView;
}

- (void)updateTabConstraints
{
    NSMutableArray *constraints = [_tabsView.constraints mutableCopy];
    for (NSInteger i = [constraints count] - 1; i >= 0; i--)
    {
        NSLayoutConstraint *constraint = [constraints objectAtIndex:i];
        if (constraint.firstItem == _leftHairlineView || constraint.secondItem == _leftHairlineView ||
            constraint.firstItem == _rightHairlineView || constraint.secondItem == _rightHairlineView)
        {
            [constraints removeObjectAtIndex:i];
        }
    }
    [_tabsView removeConstraints:constraints];
    
    if ([_tabs count] == 0)
    {
        return;
    }
    
    CGFloat tabWidth = fminf(kTabWidth, roundf(CGRectGetWidth(_tabsView.bounds) / [_tabs count]));
    CGFloat leftMargin = 0;
    for (TabView *tabView in _tabs)
    {
        [_tabsView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(leftMargin)-[tabView(tabWidth)]"
                                                                          options:0
                                                                          metrics:@{ @"leftMargin" : @(leftMargin),
                                                                                     @"tabWidth" : @(tabWidth) }
                                                                            views:NSDictionaryOfVariableBindings(tabView)]];
        [_tabsView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[tabView]-(0)-|"
                                                                          options:0
                                                                          metrics:nil
                                                                            views:NSDictionaryOfVariableBindings(tabView)]];
        leftMargin += tabWidth;
        if (leftMargin + tabWidth >= CGRectGetWidth(_tabsView.bounds))
        {
            tabWidth = CGRectGetWidth(_tabsView.bounds) - leftMargin;
        }
    }
    
    [_tabsView layoutIfNeeded];
}

- (TabView *)rightMostTab
{
    TabView *rightMostTab = nil;
    CGFloat maxLeft = -1;
    for (UIView *subview in _tabsView.subviews)
    {
        if ([subview isKindOfClass:[TabView class]] && CGRectGetMinX(subview.frame) > maxLeft)
        {
            rightMostTab = (TabView *)subview;
        }
    }
    return rightMostTab;
}

// https://developer.apple.com/library/ios/featuredarticles/ViewControllerPGforiPhoneOS/CreatingCustomContainerViewControllers/CreatingCustomContainerViewControllers.html
- (void)closeTab:(TabView *)tab
{
    if (tab == nil)
    {
        return;
    }
    [_tabs removeObject:tab];
    
    [tab.contentController willBeRemovedFromTabsController:self];
    [tab.contentController close:^{
        [_contentView removeConstraints:_contentView.constraints];
        [tab.contentController willMoveToParentViewController:nil];
        [tab.contentController.view removeFromSuperview];
        [tab.contentController removeFromParentViewController];
        [tab removeFromSuperview];
        
        [self updateTabConstraints];
        _leftHairlineView.hidden = [_tabs count] == 0;
        _rightHairlineView.hidden = _leftHairlineView.hidden;
        
        _activeTab = nil;
        [self activateTab:[self rightMostTab]];
        
        [[NSNotificationCenter defaultCenter] postNotificationName:SelectedTabDidChangeNotification object:_activeTab userInfo:nil];
    }];
}

- (void)activateTab:(TabView *)tab
{
    _leftHairlineView.hidden = [_tabs count] == 0;
    _rightHairlineView.hidden = _leftHairlineView.hidden;
    
    if (tab == nil || tab == _activeTab)
    {
        [[NSNotificationCenter defaultCenter] postNotificationName:SelectedTabDidChangeNotification object:nil];
        return;
    }
    
    [_contentView removeConstraints:_contentView.constraints];
    if (_activeTab)
    {
        [_activeTab.contentController willBeRemovedFromTabsController:self];
        [self uninstallChildViewController:_activeTab.contentController];
        _activeTab.active = NO;
    }
    _activeTab = tab;
    _activeTab.active = YES;
    
    [self installChildViewController:tab.contentController inView:_contentView];
    [tab.contentController didMoveToTabsController:self tab:tab];
    
    _leftHairlineViewWidthConstraint.constant = CGRectGetMinX(_activeTab.frame);
    _rightHairlineViewLeadingConstraint.constant = CGRectGetMaxX(_activeTab.frame);
    [_tabsView layoutIfNeeded];
    [_tabsView bringSubviewToFront:_leftHairlineView];
    [_tabsView bringSubviewToFront:_rightHairlineView];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:SelectedTabDidChangeNotification object:_activeTab];
}

- (void)activateTabController:(UIViewController<TabsViewControllerChild> *)tabController
{
    for (TabView *tab in _tabs)
    {
        if (tab.contentController == tabController)
        {
            [self activateTab:tab];
            return;
        }
    }
}

#pragma mark - UIViewController

- (void)loadView
{
    UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
    view.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.view = view;
    
    // tab bar
    _tabsView = [[UIView alloc] initWithFrame:CGRectZero];
    _tabsView.translatesAutoresizingMaskIntoConstraints = NO;
    [self.view addSubview:_tabsView];
    
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(0)-[_tabsView]-(0)-|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:NSDictionaryOfVariableBindings(_tabsView)]];
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-(0)-[_tabsView(tabHeight)]"
                                                                      options:0
                                                                      metrics:@{ @"tabHeight" : @(kTabHeight) }
                                                                        views:NSDictionaryOfVariableBindings(_tabsView)]];
    
    // hairline (separator) views
    _leftHairlineView = [[UIView alloc] initWithFrame:CGRectZero];
    _leftHairlineView.translatesAutoresizingMaskIntoConstraints = NO;
    _leftHairlineView.backgroundColor = [UIColor lightGrayColor];
    _leftHairlineView.hidden = YES;
    [_tabsView addSubview:_leftHairlineView];
    
    _rightHairlineView = [[UIView alloc] initWithFrame:CGRectZero];
    _rightHairlineView.translatesAutoresizingMaskIntoConstraints = NO;
    _rightHairlineView.backgroundColor = [UIColor lightGrayColor];
    _rightHairlineView.hidden = YES;
    [_tabsView addSubview:_rightHairlineView];
    
    CGFloat lineHeight = 1 / [UIScreen mainScreen].scale;
    // pin left hairline leading to 0
    NSLayoutConstraint *constraint = [NSLayoutConstraint constraintWithItem:_leftHairlineView
                                                                  attribute:NSLayoutAttributeLeading
                                                                  relatedBy:NSLayoutRelationEqual
                                                                     toItem:_tabsView
                                                                  attribute:NSLayoutAttributeLeft
                                                                 multiplier:1
                                                                   constant:0];
    [_tabsView addConstraint:constraint];
    
    // left hairline width constraint
    _leftHairlineViewWidthConstraint = [NSLayoutConstraint constraintWithItem:_leftHairlineView
                                                                    attribute:NSLayoutAttributeWidth
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:nil
                                                                    attribute:NSLayoutAttributeNotAnAttribute
                                                                   multiplier:1
                                                                     constant:0];
    [_tabsView addConstraint:_leftHairlineViewWidthConstraint];
    
    // pin right hairline trailing to 0
    constraint = [NSLayoutConstraint constraintWithItem:_rightHairlineView
                                              attribute:NSLayoutAttributeTrailing
                                              relatedBy:NSLayoutRelationEqual
                                                 toItem:_tabsView
                                              attribute:NSLayoutAttributeRight
                                             multiplier:1
                                               constant:0];
    [_tabsView addConstraint:constraint];
    
    // right hairline leading constraint
    _rightHairlineViewLeadingConstraint = [NSLayoutConstraint constraintWithItem:_rightHairlineView
                                                                       attribute:NSLayoutAttributeLeading
                                                                       relatedBy:NSLayoutRelationEqual
                                                                          toItem:_tabsView
                                                                       attribute:NSLayoutAttributeLeft
                                                                      multiplier:1
                                                                        constant:0];
    [_tabsView addConstraint:_rightHairlineViewLeadingConstraint];
    
    [_tabsView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_leftHairlineView(height)]-(0)-|"
                                                                      options:0
                                                                      metrics:@{ @"height" : @(lineHeight) }
                                                                        views:NSDictionaryOfVariableBindings(_leftHairlineView)]];
    [_tabsView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_rightHairlineView(height)]-(0)-|"
                                                                      options:0
                                                                      metrics:@{ @"height" : @(lineHeight) }
                                                                        views:NSDictionaryOfVariableBindings(_rightHairlineView)]];
    
    // content view
    _contentView = [[UIView alloc] initWithFrame:CGRectZero];
    _contentView.translatesAutoresizingMaskIntoConstraints = NO;
//    _contentView.layer.borderWidth = 1;
//    _contentView.layer.borderColor = [[UIColor magentaColor] CGColor];
    _contentView.clipsToBounds = YES;
    [self.view addSubview:_contentView];
    
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-(0)-[_contentView]-(0)-|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:NSDictionaryOfVariableBindings(_contentView)]];
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_tabsView]-(0)-[_contentView]"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:NSDictionaryOfVariableBindings(_tabsView, _contentView)]];
    
    _bottomConstraint = [NSLayoutConstraint constraintWithItem:self.view
                                                     attribute:NSLayoutAttributeBottom
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:_contentView
                                                     attribute:NSLayoutAttributeBottom
                                                    multiplier:1
                                                      constant:0];
    [self.view addConstraint:_bottomConstraint];
}

#pragma mark - Notifications

- (void)keyboardWillShow:(NSNotification *)notification
{
//    if (_bottomConstraint.constant > 0)
//    {
//        return;
//    }
//    CGRect keyboardFrame = [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
//    CGFloat duration = [[notification.userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
//    CGFloat keyboardHeight = fminf(CGRectGetHeight(keyboardFrame), CGRectGetWidth(keyboardFrame));
//    _bottomConstraint.constant = keyboardHeight;
//    [self.view setNeedsUpdateConstraints];
//    [UIView animateWithDuration:duration animations:^{
//        [self.view layoutIfNeeded];
//    }];
}

- (void)keyboardWillHide:(NSNotification *)notification
{
//    if (_bottomConstraint.constant == 0)
//    {
//        return;
//    }
//    CGFloat duration = [[notification.userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
//    _bottomConstraint.constant = 0;
//    [self.view setNeedsUpdateConstraints];
//    [UIView animateWithDuration:duration animations:^{
//        [self.view layoutIfNeeded];
//    }];
}


#pragma mark - Autorotation

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

@end
