//
//  DropboxViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "DropboxViewController.h"
#import "../../Core/Analytics/Analytics.h"
#import "../../Core/Dropbox/Dropbox.h"
#import "../../Core/Dropbox/DropboxSync.h"

#import <Dropbox/Dropbox.h>

static const NSInteger kInfoMaxRetryCount = 3;

@implementation DropboxViewController
{
    NSInteger infoRetryCount;
}

#pragma mark - Statics

+ (DropboxViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([DropboxViewController class]);
    DropboxViewController *dropboxController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    return dropboxController;
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Dropbox Settings"];
    
    self.title = @"Dropbox";
    
    dropboxHeaderLabel.textColor = [UIColor darkGrayColor];
    dropboxHintLabel.textColor = [UIColor darkGrayColor];
    
    dropboxHeaderLabel.text = NSLocalizedString(@"is linked to", nil);
    NSString *username = [Dropbox username];
    dropboxDisplayNameLabel.text = username ? username : NSLocalizedString(@"no account", nil);
    [dropboxLinkButton setTitle:username ? NSLocalizedString(@"Unlink", nil) : NSLocalizedString(@"Link", nil) forState:UIControlStateNormal];
    
    dropboxHintLabel.text = NSLocalizedString(@"Computable uses Dropbox to sync input and output data files.", nil);
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (self.isMovingToParentViewController)
    {
        __weak DropboxViewController *_self = self;
        [[DBAccountManager sharedManager] addObserver:self block:^(DBAccount *account) {
            [_self accountUpdated:account];
        }];
        
        infoRetryCount = 0;
        [self accountUpdated:[[DBAccountManager sharedManager] linkedAccount]];
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    if (self.isMovingFromParentViewController)
    {
        [[DBAccountManager sharedManager] removeObserver:self];
    }
    
    [super viewWillDisappear:animated];
}

#pragma mark - DropboxSettingsViewController

- (void)linkDropboxAccount:(id)sender
{
    if ([[DBAccountManager sharedManager] linkedAccount] != nil)
    {
        [Analytics trackEventWithCategory:@"Dropbox" action:@"unlink" label:nil value:nil];
        [[[DBAccountManager sharedManager] linkedAccount] unlink];
        [DropboxSync stop];
    }
    else
    {
        [Analytics trackEventWithCategory:@"Dropbox" action:@"link" label:nil value:nil];
        [[DBAccountManager sharedManager] linkFromController:self];
    }
}

- (void)accountUpdated:(DBAccount *)account
{
    if (!account.linked)
    {
        [dropboxLinkButton setTitle:NSLocalizedString(@"Link", @"Link") forState:UIControlStateNormal];
        dropboxDisplayNameLabel.text = NSLocalizedString(@"no account", @"no account");
	}
    else
    {
        [dropboxLinkButton setTitle:NSLocalizedString(@"Unlink", @"Unlink") forState:UIControlStateNormal];
        DBAccountInfo *info = account.info;
        dropboxDisplayNameLabel.text = info.userName;
        [DropboxSync start];
        if (info == nil && infoRetryCount++ <= kInfoMaxRetryCount)
        {
            double delayInSeconds = 1.0;
            dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
            dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
                [self accountUpdated:account];
            });
        }
	}
}

@end
