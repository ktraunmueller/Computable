//
//  TourViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourIntroViewController.h"
#import "Cue.h"
#import "../Core/Settings/Settings.h"

@implementation TourIntroViewController
{
    NSInteger _introStep;
    NSArray *_views;
    NSArray *_cues;
    NSArray *_topConstraints;
    NSArray *_centerXConstraints;
    NSArray *_durations;
}

#pragma mark - Statics

+ (TourIntroViewController *)newInstance
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Tour" bundle:nil];
    return [storyboard instantiateInitialViewController];
}

#pragma mark - TourViewController

- (void)applyCues:(NSArray *)cues toViews:(NSArray *)views options:(CueOptions)options
{
    [CATransaction begin];
    for (NSUInteger i = 0; i < [_views count]; i++)
    {
        NSArray *cues = _cues[i];
        UIView *view = _views[i];
        NSLayoutConstraint *topConstraint = _topConstraints[i];
        NSLayoutConstraint *centerXConstraint = _centerXConstraints[i];
        [self applyCue:cues[_introStep] to:view top:topConstraint centerX:centerXConstraint options:options];
    }
    [CATransaction commit];
}

- (void)applyCue:(Cue *)cue to:(UIView *)view top:(NSLayoutConstraint *)topConstraint centerX:(NSLayoutConstraint *)centerXConstraint options:(CueOptions)options
{
    if (cue == nil || view == nil)
    {
        return;
    }
    if (!isnan(cue.top) && (options & CueOptionTop))
    {
        topConstraint.constant = cue.top;
    }
    if (!isnan(cue.hOffset) && (options & CueOptionHOffset))
    {
        centerXConstraint.constant = cue.hOffset;
    }
    if (!isnan(cue.scale) && (options & CueOptionScale))
    {
        view.transform = CGAffineTransformMakeScale(cue.scale, cue.scale);
    }
    if (!isnan(cue.alpha) && (options & CueOptionAlpha))
    {
        view.alpha = cue.alpha;
    }
}

- (void)introStepForward:(BOOL)forward delay:(NSTimeInterval)delay completion:(dispatch_block_t)completion
{
    NSInteger numberOfSteps = [_cues[0] count];
    _introStep = (_introStep + (forward ? 1 : -1) + numberOfSteps) % numberOfSteps;
    NSTimeInterval duration = [_durations[_introStep] doubleValue];
    
    [UIView animateWithDuration:duration
                          delay:delay
         usingSpringWithDamping:0.85
          initialSpringVelocity:0
                        options:0
                     animations:^{
                         [self applyCues:_cues toViews:_views options:CueOptionAll];
                         [self.view layoutIfNeeded];
                     } completion:^(BOOL finished) {
                         [self.view layoutIfNeeded];
                         if (finished && completion)
                         {
                             completion();
                         }
                     }];
}

- (IBAction)startTour:(id)sender
{
    if (_startTourBlock)
    {
        _startTourBlock();
    }
}

- (IBAction)skipTour:(id)sender
{
    if (_skipTourBlock)
    {
        _skipTourBlock();
    }
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    FontSetting *font = [Settings instance].modernTextFont;
    _welcomeLabel.font = [font.boldFont fontWithSize:80];
    _computableLabel.font = [font.regularFont fontWithSize:55];
    _startLabel.font = [font.regularFont fontWithSize:28];
    
    _iconView.layer.cornerRadius = 15;
    _iconView.clipsToBounds = YES;
    
    _sureButton.layer.cornerRadius = 4;
    _sureButton.backgroundColor = [UIColor colorWithRed:132/255.0 green:204/255.0 blue:28/255.0 alpha:1.0];
    _sureButton.titleLabel.font = [UIFont boldSystemFontOfSize:20];
    //    _sureButton.titleEdgeInsets = UIEdgeInsetsMake(20, 20, 20, 20);
    [_sureButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    _welcomeLabel.text = NSLocalizedString(@"Welcome", nil);
    _computableLabel.text = NSLocalizedString(@"to Computable", nil);
    _startLabel.text = NSLocalizedString(@"Computable offers some exciting features.\nTake a quick tour to see them in action!", nil);
    [_noThanksButton setTitle:NSLocalizedString(@"Skip Tour", nil) forState:UIControlStateNormal];
    [_sureButton setTitle:NSLocalizedString(@"Start Tour", nil) forState:UIControlStateNormal];
    
    NSArray *welcomeCues = @[ [Cue cueWithTop:150 hOffset:1000 scale:1 alpha:0],
                              [Cue cueWithTop:150 hOffset:20 scale:1 alpha:1],
                              [Cue cueWithTop:iOS8_OR_LATER ? 116.5 : 123.5 hOffset:110 scale:0.4 alpha:1],
                              kNopCue,
                              ];
    NSArray *computableCues = @[ [Cue cueWithTop:250 hOffset:-1000 scale:1 alpha:0],
                                 [Cue cueWithTop:250 hOffset:50 scale:1 alpha:1],
                                 [Cue cueWithTop:122 hOffset:-52 scale:0.55 alpha:1],
                                 kNopCue,
                                 ];
    NSArray *iconCues = @[ [Cue cueWithTop:254 hOffset:-1000 scale:1 alpha:0],
                           [Cue cueWithTop:250 hOffset:-165 scale:1 alpha:1],
                           [Cue cueWithTop:125 hOffset:-175 scale:0.5 alpha:1],
                           kNopCue,
                           ];
    NSArray *startCues = @[ [Cue cueWithTop:450 hOffset:0 scale:1 alpha:0],
                            kNopCue,
                            [Cue cueWithTop:300 hOffset:0 scale:1 alpha:1],
                            [Cue cueWithTop:280 hOffset:0 scale:1 alpha:1],
                            ];
    NSArray *noThanksCues = @[ [Cue cueWithTop:505 hOffset:700 scale:1 alpha:0],
                               kNopCue,
                               kNopCue,
                               [Cue cueWithTop:505 hOffset:100 scale:1 alpha:1],
                               ];
    NSArray *sureCues = @[ [Cue cueWithTop:500 hOffset:-700 scale:1 alpha:0],
                           kNopCue,
                           kNopCue,
                           [Cue cueWithTop:500 hOffset:-60 scale:1 alpha:1],
                           ];
    
    _views = @[ _welcomeLabel,
                _computableLabel,
                _iconView,
                _startLabel,
                _noThanksButton,
                _sureButton ];
    _cues = @[ welcomeCues,
               computableCues,
               iconCues,
               startCues,
               noThanksCues,
               sureCues ];
    _durations = @[ @0, @1.2, @1.2, @0.6 ];
    _topConstraints = @[ _welcomeTopConstraint,
                         _computableTopConstraint,
                         _iconTopConstraint,
                         _startTopConstraint,
                         _noThanksTopConstraint,
                         _sureTopConstraint ];
    _centerXConstraints = @[ _welcomeCenterXConstraint,
                             _computableCenterXConstraint,
                             _iconCenterXConstraint,
                             _startCenterXConstraint,
                             _noThanksCenterXConstraint,
                             _sureCenterXConstraint ];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    _introStep = 0;
    [self applyCues:_cues toViews:_views options:CueOptionAll];
    [self.view layoutIfNeeded];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [self introStepForward:YES delay:0.5 completion:^{
        [self introStepForward:YES delay:0.8 completion:^{
            [self introStepForward:YES delay:0.8 completion:^{
                // highlight start button
            }];
        }];
    }];
}

#pragma mark - UIViewController (UIViewControllerRotation)

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

@end
