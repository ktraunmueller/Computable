//
//  TourStageViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 12.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourStageViewController.h"
#import "TourPage.h"
#import "TourPageViewController.h"
#import "TourPage1ControllerDelegate.h"
#import "TourPage2ControllerDelegate.h"
#import "TourPage3ControllerDelegate.h"
#import "TourPage4ControllerDelegate.h"
#import "TourPage5ControllerDelegate.h"
#import "TourPage6ControllerDelegate.h"
#import "TourPage7ControllerDelegate.h"
#import "TourPage8ControllerDelegate.h"
#import "AnimationStep.h"
#import "../Core/Settings/Settings.h"
#import "../../Common/Utilities/UIViewController+Containment.h"
#import "../../Common/Utilities/UIView+LayoutDiagnostics.h"

static const NSTimeInterval kTransitionDuration = 0.7;
static const NSTimeInterval kPageAnimationsDelay = 5;
static const CGFloat kUnfocusedThumbAlpha = 0.5;

static const CGFloat kWelcomeTopPortrait = 60;
static const CGFloat kWelcomeTopLandscape = 40;
static const CGFloat kTitleTopPortrait = 150;
static const CGFloat kTitleTopLandscape = 95;
static const CGFloat kScrollViewTopPortrait = 280;
static const CGFloat kScrollViewTopLandscape = 190;

@interface TourStageViewController () <UIScrollViewDelegate>
@end

@implementation TourStageViewController
{
    NSInteger _pageIndex;
    NSTimer *_pageAnimationStartTimer;
    NSMutableArray *_pages;
    NSArray *_thumbViews;
    UIColor *_thumbBorderColor;
}

#pragma mark - Statics

+ (TourStageViewController *)newInstance
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Tour" bundle:nil];
    return [storyboard instantiateViewControllerWithIdentifier:NSStringFromClass([self class])];
}

#pragma mark - TourStageViewController

- (void)activatePageAtIndex:(NSInteger)index animated:(BOOL)animated
{
    [self unfocusThumbAtIndex:_pageIndex animated:animated];
    [self focusThumbAtIndex:index animated:animated];
    [self scrollToPageAtIndex:index animated:animated];
}

- (void)scrollToPageAtIndex:(NSInteger)index animated:(BOOL)animated
{
    if (index < 0 || index >= [_pages count])
    {
        return;
    }
    _pageIndex = index;
    TourPage *page = _pages[_pageIndex];
    [page.pageViewController reset];
    _progressView.progress = 0;
    if (!animated)
    {
        _titleLabel.text = page.title;
        _titleLabel.transform = CGAffineTransformIdentity;
        [self startPageAnimations];
        return;
    }
    [_titleLabel.layer removeAllAnimations];
    CGFloat scale = 0.99;
    NSTimeInterval duration = kTransitionDuration;
    [UIView animateWithDuration:duration / 3
                          delay:0
                        options:UIViewAnimationOptionCurveEaseInOut
                     animations:^{
                         _titleLabel.alpha = 0.1;
                     } completion:^(BOOL finished) {
                         _titleLabel.text = page.title;
                         _titleLabel.transform = CGAffineTransformMakeScale(scale, scale);
                         [UIView animateWithDuration:duration / 3
                                               delay:0
                                             options:UIViewAnimationOptionCurveEaseInOut
                                          animations:^{
                                              _titleLabel.alpha = 1;
                                              _titleLabel.transform = CGAffineTransformIdentity;
                                          } completion:^(BOOL finished) {
                                              [self startPageAnimations];
                                          }];
                     }];
}

- (void)startPageAnimations
{
    if (_pageIndex < 0 || _pageIndex >= [_pages count])
    {
        return;
    }
    TourPage *page = _pages[_pageIndex];
    _progressView.progress = 0;
    [_pageAnimationStartTimer invalidate];
    _pageAnimationStartTimer = [NSTimer scheduledTimerWithTimeInterval:page.animationsDelay
                                                                target:self
                                                              selector:@selector(pageAnimationStartTimerFired:)
                                                              userInfo:nil
                                                               repeats:NO];
}

- (void)pageAnimationStartTimerFired:(NSTimer *)timer
{
    [timer invalidate];
    
    TourPage *page = _pages[_pageIndex];
    [page.pageViewController start];
    if ([page.pageViewController.delegate.animationSteps count] > 0)
    {
        [_progressView setProgress:0 animated:NO];
        [UIView animateWithDuration:[AnimationStep totalDuration:page.pageViewController.delegate.animationSteps]
                              delay:0
                            options:UIViewAnimationOptionCurveLinear
                         animations:^{
                             [_progressView setProgress:1 animated:YES];
                         } completion:nil];
    }
    
}

- (void)scalePages
{
    CGFloat scrollPosition = _scrollView.contentOffset.x;
    CGFloat pageWidth = CGRectGetWidth(_scrollView.bounds);
    CGFloat scrollCenter = scrollPosition + pageWidth / 2;
    
    for (TourPage *page in _pages)
    {
        CGFloat x = page.pageContainerView.center.x;
        CGFloat distance = fabs(x - scrollCenter);
        CGFloat minScale = 0.5;
        CGFloat scale = minScale;
        if (distance < pageWidth)
        {
            scale = distance < 5 ? 1 : sqrt(0.3 * pageWidth / distance);
            scale = fmaxf(minScale, fminf(1, scale));
        }
        page.pageContainerView.transform = CGAffineTransformMakeScale(scale, scale);
    }
}

- (void)focusThumbAtIndex:(NSInteger)index animated:(BOOL)animated
{
    [self focusThumb:YES atIndex:index animated:animated];
}

- (void)unfocusThumbAtIndex:(NSInteger)index animated:(BOOL)animated
{
    [self focusThumb:NO atIndex:index animated:animated];
}

- (void)focusThumb:(BOOL)focus atIndex:(NSInteger)index animated:(BOOL)animated
{
    if (index < 0 || index >= [_pages count])
    {
        return;
    }
    TourPage *page = _pages[index];
    NSTimeInterval duration = kTransitionDuration;
    [page.thumbView.layer removeAllAnimations];
    [UIView animateWithDuration:animated ? duration : 0
                          delay:0
         usingSpringWithDamping:0.8
          initialSpringVelocity:0
                        options:0
                     animations:^{
                         page.thumbView.layer.borderWidth = focus ? 1 : 0.5;
                         page.thumbView.alpha = focus ? 1 : kUnfocusedThumbAlpha;
                     } completion:nil];
}

- (void)updateLayoutForOrientation:(UIInterfaceOrientation)orientation duration:(NSTimeInterval)duration
{
    BOOL portrait = UIInterfaceOrientationIsPortrait(orientation);
    _welcomeTopConstraint.constant = portrait ? kWelcomeTopPortrait : kWelcomeTopLandscape;
    _titleTopConstraint.constant = portrait ? kTitleTopPortrait : kTitleTopLandscape;
    _scrollViewTopConstraint.constant = portrait ? kScrollViewTopPortrait : kScrollViewTopLandscape;
    
    [UIView animateWithDuration:duration animations:^{
        [self.view layoutIfNeeded];
    }];
}

- (void)thumbTapped:(UITapGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        UIView *thumb = recognizer.view;
        NSInteger index = [_thumbViews indexOfObject:thumb];
        NSInteger timeScale = index != _pageIndex ? sqrt(abs(index - _pageIndex)) : 1;
        [UIView animateWithDuration:timeScale * kTransitionDuration animations:^{
            [_scrollView setContentOffset:CGPointMake(index * CGRectGetWidth(_scrollView.bounds), 0) animated:NO];
        }];
    }
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    FontSetting *font = [Settings instance].modernTextFont;
    _welcomeLabel.font = [font.boldFont fontWithSize:22];
    _welcomeLabel.textColor = [UIColor colorWithWhite:0 alpha:1.0];
    _titleLabel.font = [font.regularFont fontWithSize:20];
    _titleLabel.textColor = [UIColor colorWithWhite:0.3 alpha:1.0];
    _hintLabel.font = [font.regularFont fontWithSize:12];
    _hintLabel.textColor = [UIColor colorWithWhite:0.5 alpha:1.0];
    
    _iconView.layer.cornerRadius = 5;
    _iconView.clipsToBounds = YES;
    
    _thumbBorderColor = [UIColor colorWithWhite:0.7 alpha:1.0];
    
    _scrollView.translatesAutoresizingMaskIntoConstraints = NO;
    _scrollView.delegate = self;
    _scrollView.pagingEnabled = YES;
    _scrollView.showsHorizontalScrollIndicator = NO;
    _scrollView.layer.borderWidth = 0.5;
    _scrollView.layer.borderColor = _thumbBorderColor.CGColor;
    
    _progressView.progress = 0;
    _progressView.progressTintColor = [self.view.tintColor colorWithAlphaComponent:0.5];
    
    _thumbViews = @[ _thumb1View, _thumb2View, _thumb3View, _thumb4View, _thumb5View, _thumb6View, _thumb7View, _thumb8View ];
    _pages = [NSMutableArray array];
    CGSize pageSize = _scrollView.bounds.size;
    for (UIView *thumbView in _thumbViews)
    {
        thumbView.backgroundColor = [UIColor clearColor];
        thumbView.layer.borderWidth = 0.5;
        thumbView.layer.borderColor = _thumbBorderColor.CGColor;
        thumbView.alpha = kUnfocusedThumbAlpha;
        [thumbView addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(thumbTapped:)]];
        
        NSInteger pageIndex = [_pages count];
        
        TourPage *page = [TourPage new];
        [_pages addObject:page];
        page.thumbView = thumbView;
        page.animationsDelay = kPageAnimationsDelay;
        
        UIImageView *thumbImageView = [[UIImageView alloc] initWithFrame:thumbView.bounds];
        thumbImageView.contentMode = UIViewContentModeScaleAspectFit;
        page.thumbImageView = thumbImageView;
        [thumbView addSubview:thumbImageView];
        
        page.pageContainerView = [[UIView alloc] initWithFrame:CGRectMake(pageIndex * pageSize.width, 0, pageSize.width, pageSize.height)];
        page.pageContainerView.backgroundColor = [UIColor clearColor];
        [_scrollView addSubview:page.pageContainerView];
        
        TourPageViewController *pageViewController = [self.storyboard instantiateViewControllerWithIdentifier:NSStringFromClass([TourPageViewController class])];
        pageViewController.tourPage = page;
        page.pageViewController = pageViewController;
        
        page.thumbImageName = [NSString stringWithFormat:@"tour%d_thumb", pageIndex + 1];
        switch (pageIndex)
        {
            case 0:
                page.title = NSLocalizedString(@" Computable is a full-featured IPython system for iPad.\nIf you don’t know IPython yet, it's an interactive environment\nfor scientific and engineering calculations.", nil);
                page.imageName = @"tour1";
                pageViewController.delegate = [TourPage1ControllerDelegate new];
                break;
            case 1:page.title = NSLocalizedString(@"Computable comes with a big collection of algorithms and functions known as the SciPy stack. That’s your problem-solving toolbox.\nA rather large toolbox, for that matter.", nil);
                page.imageName = @"tour2";
                page.animationsDelay = 4;
                pageViewController.delegate = [TourPage2ControllerDelegate new];
                break;
            case 2:
                page.title = NSLocalizedString(@"The unit of work in IPython is the notebook. Notebooks are comprised of cells, which may contain Python code, Markdown-formatted text, TeX- or LaTeX-formatted maths, or inline graphics.", nil);
                page.imageName = @"tour3";
                page.animationsDelay = 6;
                pageViewController.delegate = [TourPage3ControllerDelegate new];
                break;
            case 3:
                page.title = NSLocalizedString(@"Interacting with cells is straightforward.\nTo select a cell, tap it once. To edit a selected cell, tap it again.\nThe checkmark toolbar button will commit your edits.", nil);
                page.videoPath = [[NSBundle mainBundle] pathForResource:@"Tour4" ofType:@"mov"];
                page.animationsDelay = 6;
                pageViewController.delegate = [TourPage4ControllerDelegate new];
                break;
            case 4:
                page.title = NSLocalizedString(@"Computable offers augmented keyboards for Python and Markdown editing. Note that the top portion of the keyboard can be swiped horizontally to reveal a virtual touchpad for quick caret positioning.", nil);
                page.videoPath = [[NSBundle mainBundle] pathForResource:@"Tour5" ofType:@"mov"];
                page.animationsDelay = 6;
                pageViewController.delegate = [TourPage5ControllerDelegate new];
                break;
            case 5:
                page.title = NSLocalizedString(@"New in version 1.1 is Python code assist, offering completions and inline documentation. Code assist is available through the floating i-Button.", nil);
                page.videoPath = [[NSBundle mainBundle] pathForResource:@"Tour6" ofType:@"mov"];
                pageViewController.delegate = [TourPage6ControllerDelegate new];
                break;
            case 6:
                page.title = NSLocalizedString(@"Computable comes with a number of sample notebooks\nto try it out before you make a purchase decision.", nil);
                page.videoPath = [[NSBundle mainBundle] pathForResource:@"Tour7" ofType:@"mov"];
                page.animationsDelay = 4;
                pageViewController.delegate = [TourPage7ControllerDelegate new];
                break;
            case 7:
                page.title = NSLocalizedString(@"Thanks for taking the tour!\nFollow @computableapp on Twitter for news and updates.", nil);
                page.imageName = @"tour8";
                page.animationsDelay = 3;
                pageViewController.delegate = [TourPage8ControllerDelegate new];
                break;
            default:
                break;
        }
        
        [pageViewController.delegate setup];
        [self installChildViewController:pageViewController inView:page.pageContainerView];
        page.thumbImageView.image = [UIImage imageNamed:page.thumbImageName];
    }
    _scrollView.contentSize = CGSizeMake([_pages count] * pageSize.width, pageSize.height);
    
    _welcomeLabel.text = NSLocalizedString(@"Welcome to Computable", nil);
    _hintLabel.text = NSLocalizedString(@"Tap a thumbnail or swipe left/right to move through the tour.", nil);
    
//    [self.view enableLayoutDiagnostics:YES];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self updateLayoutForOrientation:[UIApplication sharedApplication].statusBarOrientation duration:0];
    
    for (TourPage *page in _pages)
    {
        [page.pageViewController reset];
    }
    _scrollView.contentOffset = CGPointZero;
    [self scrollToPageAtIndex:0 animated:NO];
    [self focusThumbAtIndex:0 animated:NO];
    [self scalePages];
}

#pragma mark - UIViewController (UIViewControllerRotation)

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [self updateLayoutForOrientation:toInterfaceOrientation duration:duration];
}

#pragma mark - UIScrollViewDelegate

//- (int)centerIndex
//{
//    return (int)(floorf(_scrollView.contentOffset.x / CGRectGetWidth(_scrollView.bounds) + 0.5) + 1) % [_labels count];
//}


- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    [self scalePages];
    
    int pageIndex = roundf(_scrollView.contentOffset.x / CGRectGetWidth(_scrollView.bounds));
    if (pageIndex != _pageIndex)
    {
        [self activatePageAtIndex:pageIndex animated:YES];
    }
}

@end
