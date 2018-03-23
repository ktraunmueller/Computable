//
//  TabView.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TabsViewController.h"

@class TabsViewController;

@interface TabView : UIView

@property (nonatomic, strong) UIViewController<TabsViewControllerChild> *contentController;
@property (nonatomic, weak) TabsViewController *tabsController;
@property (nonatomic, readonly) UILabel *label;
@property (nonatomic, assign) BOOL active;

- (void)startSpinner;
- (void)stopSpinner;

@end
