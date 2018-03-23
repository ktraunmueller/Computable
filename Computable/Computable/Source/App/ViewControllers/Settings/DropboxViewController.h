//
//  DropboxViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DropboxViewController : UIViewController
{
@private
    IBOutlet UILabel *dropboxHeaderLabel;
    IBOutlet UILabel *dropboxDisplayNameLabel;
    IBOutlet UIButton *dropboxLinkButton;
    IBOutlet UILabel *dropboxHintLabel;
}

+ (DropboxViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard;

- (IBAction)linkDropboxAccount:(id)sender;

@end
