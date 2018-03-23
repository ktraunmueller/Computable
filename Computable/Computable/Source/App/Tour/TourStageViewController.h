//
//  TourStageViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 12.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TourStageViewController : UIViewController
{
@private
    __weak IBOutlet UILabel *_welcomeLabel;
    __weak IBOutlet NSLayoutConstraint *_welcomeTopConstraint;
    __weak IBOutlet UIImageView *_iconView;
    __weak IBOutlet UILabel *_titleLabel;
    __weak IBOutlet NSLayoutConstraint *_titleTopConstraint;
    __weak IBOutlet UIProgressView *_progressView;
    __weak IBOutlet UIScrollView *_scrollView;
    __weak IBOutlet NSLayoutConstraint *_scrollViewTopConstraint;
    __weak IBOutlet UIView *_thumb1View;
    __weak IBOutlet UIView *_thumb2View;
    __weak IBOutlet UIView *_thumb3View;
    __weak IBOutlet UIView *_thumb4View;
    __weak IBOutlet UIView *_thumb5View;
    __weak IBOutlet UIView *_thumb6View;
    __weak IBOutlet UIView *_thumb7View;
    __weak IBOutlet UIView *_thumb8View;
    __weak IBOutlet UILabel *_hintLabel;
}

+ (TourStageViewController *)newInstance;

@end
