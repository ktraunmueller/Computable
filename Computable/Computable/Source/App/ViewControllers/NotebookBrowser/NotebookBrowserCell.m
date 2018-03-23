//
//  NotebookBrowserCell.m
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookBrowserCell.h"
#import "../../../Common/Utilities/UIView+Wiggle.h"

NSString *const NotebookBrowserCellDidRequestDeleteNotification = @"NotebookBrowserCellDidRequestDeleteNotification";

static const NSTimeInterval kFadeAnimationDuration = 0.2;
static const CGFloat kScale = 0.7;

@implementation NotebookBrowserCell
{
    UITapGestureRecognizer *_tapRecognizer;
    BOOL _ignoreWiggling;
}

#pragma mark - NotebookBrowserCell

- (void)startWiggling
{
    if (_ignoreWiggling || [self isWiggleAnimationRunning])
    {
        return;
    }
    
    _tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(deleteButtonTapped:)];
    [self.deleteTarget addGestureRecognizer:_tapRecognizer];
    
    [self startWiggleAnimation];
    self.deleteButton.alpha = 0;
    self.deleteButton.hidden = NO;
    self.deleteButton.layer.transform = CATransform3DMakeScale(kScale, kScale, 1.0);
    self.deleteButtonBackgroundView.alpha = self.deleteButton.alpha;
    self.deleteButtonBackgroundView.hidden = self.deleteButton.hidden;
    self.deleteButtonBackgroundView.transform = self.deleteButton.transform;
    [UIView animateWithDuration:kFadeAnimationDuration animations:^{
        self.deleteButton.alpha = 1;
        self.deleteButton.layer.transform = CATransform3DIdentity;
        self.deleteButtonBackgroundView.alpha = self.deleteButton.alpha;
        self.deleteButtonBackgroundView.transform = self.deleteButton.transform;
    }];
}

- (void)stopWiggling
{
    if (![self isWiggleAnimationRunning])
    {
        return;
    }
    
    [self.deleteTarget removeGestureRecognizer:_tapRecognizer];
    _tapRecognizer = nil;
    
    [UIView animateWithDuration:kFadeAnimationDuration animations:^{
        self.deleteButton.alpha = 0;
        self.deleteButton.layer.transform = CATransform3DMakeScale(kScale, kScale, 1.0);
        self.deleteButtonBackgroundView.alpha = self.deleteButton.alpha;
        self.deleteButtonBackgroundView.transform = self.deleteButton.transform;
    } completion:^(BOOL finished) {
        if (!finished)
        {
            return;
        }
        self.deleteButton.hidden = YES;
        self.deleteButtonBackgroundView.hidden = YES;
        [self stopWiggleAnimation];
    }];
}

- (void)ignoreWiggling:(BOOL)ignore
{
    _ignoreWiggling = ignore;
}

- (void)deleteButtonTapped:(id)sender
{
    [[NSNotificationCenter defaultCenter] postNotificationName:NotebookBrowserCellDidRequestDeleteNotification object:self];
}

#pragma mark - NSObject(UINibLoadingAdditions)

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    _titleField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
}

@end
