//
//  DropboxBrowserViewController.h
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 17.07.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^DropboxBrowserCompletion)(NSArray *selectedFiles);

@class DBFilesystem;
@class DBPath;
@class DBFileInfo;

@interface DropboxBrowserViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>
{
@private
    IBOutlet UITableView *_tableView;
    IBOutlet UIView *_notConnectedView;
    IBOutlet UILabel *_notConnectedHintLabel;
}

@property (nonatomic, assign) BOOL singleSelection;
@property (nonatomic, weak) DropboxBrowserViewController *parentBrowser;
@property (nonatomic, strong) DropboxBrowserCompletion completion;
@property (nonatomic, readonly) DropboxBrowserViewController *rootBrowser;
@property (nonatomic, copy) NSArray *suppressedExtensions;

+ (DropboxBrowserViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard;

- (void)dismiss:(id)sender;

@end
