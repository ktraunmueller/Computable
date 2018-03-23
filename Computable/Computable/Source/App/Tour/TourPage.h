//
//  TourPage.h
//  Computable
//
//  Created by Karl Traunmüller on 12.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@class TourPageViewController;

@interface TourPage : NSObject

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *thumbImageName;
@property (nonatomic, copy) NSString *imageName;
@property (nonatomic, copy) NSString *videoPath;
@property (nonatomic, strong) TourPageViewController *pageViewController;
@property (nonatomic, strong) UIView *pageContainerView;
@property (nonatomic, strong) UIView *thumbView;
@property (nonatomic, strong) UIImageView *thumbImageView;
@property (nonatomic, assign) NSTimeInterval animationsDelay;

@end
