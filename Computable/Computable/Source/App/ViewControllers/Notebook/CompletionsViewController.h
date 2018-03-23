//
//  CompletionsViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 06.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "../../Core/Python/Editor/Completion.h"

extern NSString *const CompletionsPopupDidSelectCompletionNotification;
extern NSString *const CompletionsPopupDidDismissNotification;

extern const CGFloat kCompletionsWindowWidth;

extern Completion *kLoadingCompletion;

@protocol CompletionsViewControllerDelegate;

@interface CompletionsViewController : UIViewController
{
@private
    __weak IBOutlet UITableView *_tableView;
    __weak IBOutlet UIActivityIndicatorView *_loadingSpinner;
    __weak IBOutlet UILabel *_loadingLabel;
}

@property (nonatomic, weak) id<CompletionsViewControllerDelegate> completionsDelegate;

+ (CompletionsViewController *)instance;

- (BOOL)popoverVisible;
- (void)movePopoverToRect:(CGRect)rect inView:(UIView *)view;
- (void)showCompletions:(NSArray *)completions fromRect:(CGRect)rect inView:(UIView *)view;
- (void)dismissAnimated:(BOOL)animated;

@end
