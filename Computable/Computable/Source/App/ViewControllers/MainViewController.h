//
//  ViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MainViewController : UIViewController
{
@private
    __weak IBOutlet UIView *_statusPanelView;
    __weak IBOutlet UILabel *_statusLabel;
    __weak IBOutlet NSLayoutConstraint *_statusPanelTopConstraint;
}

- (void)hideStatusBar:(BOOL)hide;
- (void)dismissOpenModalViewControllerWithCompletion:(dispatch_block_t)completion;

@end
