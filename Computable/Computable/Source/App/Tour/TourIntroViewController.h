//
//  TourViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TourIntroViewController : UIViewController
{
@private
    __weak IBOutlet UILabel *_welcomeLabel;
    __weak IBOutlet NSLayoutConstraint *_welcomeTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_welcomeCenterXConstraint;
    
    __weak IBOutlet UILabel *_computableLabel;
    __weak IBOutlet NSLayoutConstraint *_computableTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_computableCenterXConstraint;
    
    __weak IBOutlet UIImageView *_iconView;
    __weak IBOutlet NSLayoutConstraint *_iconTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_iconCenterXConstraint;
    
    __weak IBOutlet UILabel *_startLabel;
    __weak IBOutlet NSLayoutConstraint *_startTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_startCenterXConstraint;
    
    __weak IBOutlet UIButton *_noThanksButton;
    __weak IBOutlet NSLayoutConstraint *_noThanksTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_noThanksCenterXConstraint;
    
    __weak IBOutlet UIButton *_sureButton;
    __weak IBOutlet NSLayoutConstraint *_sureTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_sureCenterXConstraint;
    __weak IBOutlet NSLayoutConstraint *_sureWidthConstraint;
    __weak IBOutlet NSLayoutConstraint *_sureHeightConstraint;
}

@property (nonatomic, copy) dispatch_block_t startTourBlock;
@property (nonatomic, copy) dispatch_block_t skipTourBlock;

+ (TourIntroViewController *)newInstance;

- (IBAction)startTour:(id)sender;
- (IBAction)skipTour:(id)sender;

@end
