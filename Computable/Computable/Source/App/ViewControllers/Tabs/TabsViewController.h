//
//  TabsViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const SelectedTabDidChangeNotification;

@class TabView;
@protocol TabsViewControllerChild;

@interface TabsViewController : UIViewController

+ (TabsViewController *)newInstance;

@property (nonatomic, readonly) NSArray *tabs;
@property (nonatomic, readonly) TabView *activeTab;

- (TabView *)addTabController:(UIViewController<TabsViewControllerChild> *)tabController;

- (void)closeTab:(TabView *)tab;
- (void)activateTab:(TabView *)tab;
- (void)activateTabController:(UIViewController<TabsViewControllerChild> *)tabController;
    
@end

@protocol TabsViewControllerChild <NSObject>

- (void)didMoveToTabsController:(TabsViewController *)tabsController tab:(TabView *)tab;
- (void)willBeRemovedFromTabsController:(TabsViewController *)tabsController;

- (void)close:(dispatch_block_t)completion;

@end
