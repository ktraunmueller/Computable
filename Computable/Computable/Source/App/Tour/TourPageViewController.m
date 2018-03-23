//
//  TourPageViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPageViewController.h"
#import "TourPageControllerDelegate.h"
#import "TourPage.h"
#import "AnimationStep.h"
#import "../../Common/Views/VideoPlayerView.h"
#import "../../Common/Utilities/UIView+LayoutDiagnostics.h"

NSString *const EndTourNotification = @"EndTourNotification";

@implementation TourPageViewController
{
    NSUInteger _stepIndex;
    VideoPlayerView *_playerView;
}

#pragma mark - Properties

- (UIView *)videoPlayerView
{
    return _playerView;
}

#pragma mark - TourPageViewController

- (void)reset
{
    _stepIndex = 0;
    [self.delegate resetPageController:self];
    [self.view.layer removeAllAnimations];
    
    if (_tourPage.videoPath)
    {
        if (!_playerView.isVideoLoaded)
        {
            [_playerView loadVideo:self.tourPage.videoPath];
        }
        [_playerView stop];
        [_playerView rewind];
    }
}

- (void)start
{
    [self scheduleNextAnimationStep];
    
    if (_tourPage.videoPath)
    {
        [_playerView start];
    }
}

- (void)scheduleNextAnimationStep
{
    if (_stepIndex >= [self.delegate.animationSteps count])
    {
        return;
    }
    AnimationStep *step = self.delegate.animationSteps[_stepIndex];
    [self.delegate willPerformStep:step inPageController:self];
    [UIView animateWithDuration:step.duration
                          delay:step.delay
                        options:step.options
                     animations:^{
                         [self.delegate performStep:step inPageController:self];
                     } completion:^(BOOL finished) {
                         if (finished)
                         {
                             _stepIndex++;
                             [self scheduleNextAnimationStep];
                         }
                     }];
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _imageView.contentMode = UIViewContentModeCenter;
    
    if (_tourPage.videoPath)
    {
        _playerView = [[VideoPlayerView alloc] initWithFrame:CGRectMake(0, 0, 768, 1024)];
        _playerView.translatesAutoresizingMaskIntoConstraints = NO;
        [self.view addSubview:_playerView];
        
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:_playerView
                                                                   attribute:NSLayoutAttributeCenterX
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:self.imageView
                                                                   attribute:NSLayoutAttributeCenterX
                                                                  multiplier:1
                                                                    constant:0]];
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:_playerView
                                                                   attribute:NSLayoutAttributeCenterY
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:self.imageView
                                                                   attribute:NSLayoutAttributeCenterY
                                                                  multiplier:1
                                                                    constant:0]];
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:_playerView
                                                                   attribute:NSLayoutAttributeWidth
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:nil
                                                                   attribute:NSLayoutAttributeNotAnAttribute
                                                                  multiplier:1
                                                                    constant:768]];
        [self.view addConstraint:[NSLayoutConstraint constraintWithItem:_playerView
                                                                   attribute:NSLayoutAttributeHeight
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:nil
                                                                   attribute:NSLayoutAttributeNotAnAttribute
                                                                  multiplier:1
                                                                    constant:1024]];
        
//        [self.view enableLayoutDiagnostics:YES borderColor:[UIColor greenColor]];
//        [_playerView enableLayoutDiagnostics:YES borderColor:[UIColor redColor]];
    }
}

@end
