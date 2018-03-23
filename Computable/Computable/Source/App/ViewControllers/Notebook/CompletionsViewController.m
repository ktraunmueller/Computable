//
//  CompletionsViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 06.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "CompletionsViewController.h"
#import "DocsViewController.h"
#import "../../Core/Python/Editor/Completion.h"
#import "../../Core/Settings/Settings.h"

NSString *const CompletionsPopupDidSelectCompletionNotification = @"CompletionsPopupDidSelectCompletionNotification";
NSString *const CompletionsPopupDidDismissNotification = @"CompletionsPopupDidDismissNotification";

const CGFloat kCompletionsWindowWidth = 420;

static NSString *const kCompletionCellIdentifier = @"CompletionCell";

static const NSTimeInterval kFadeDuration = 0.2;
static const CGFloat kFadeAlpha = 0.2;

@interface CompletionsViewController () <UITableViewDataSource, UITableViewDelegate, UIPopoverControllerDelegate>
@end

Completion *kLoadingCompletion;

static CompletionsViewController *instance;

@implementation CompletionsViewController
{
    UIPopoverController *_popover;
    NSArray *_completions;
}

#pragma mark - Statics

+ (CompletionsViewController *)instance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        kLoadingCompletion = [Completion new];
        instance = [[UIStoryboard storyboardWithName:@"Main" bundle:nil]
                    instantiateViewControllerWithIdentifier:NSStringFromClass( [CompletionsViewController class])];
    });
    return instance;
}

#pragma mark - CompletionsViewController

- (BOOL)popoverVisible
{
    return [_popover isPopoverVisible];
}

- (void)movePopoverToRect:(CGRect)rect inView:(UIView *)view
{
    if (![_popover isPopoverVisible])
    {
        return;
    }
    [_popover presentPopoverFromRect:rect inView:view permittedArrowDirections:UIPopoverArrowDirectionLeft | UIPopoverArrowDirectionRight animated:NO];
}

- (void)showCompletions:(NSArray *)completions fromRect:(CGRect)rect inView:(UIView *)view
{
    _completions = completions;
    _tableView.dataSource = self;
    _tableView.delegate = self;
    
    BOOL showLoading = [_completions count] == 0 || _completions.firstObject == kLoadingCompletion;
    
    // show loading or update
    if ([_popover isPopoverVisible])
    {
        // move
        [_popover presentPopoverFromRect:rect inView:view permittedArrowDirections:UIPopoverArrowDirectionLeft | UIPopoverArrowDirectionRight animated:NO];
        if (showLoading)
        {
            [UIView animateWithDuration:kFadeDuration animations:^{
                _loadingLabel.alpha = 1;
                _tableView.alpha = kFadeAlpha;
            }];
            [_loadingSpinner startAnimating];
            _tableView.userInteractionEnabled = NO;
        }
        else
        {
            [_loadingSpinner stopAnimating];
            _tableView.userInteractionEnabled = YES;
            [UIView animateWithDuration:kFadeDuration animations:^{
                _loadingLabel.alpha = 0;
                _tableView.alpha = 1;
            }];
            [_tableView reloadData];
        }
        return;
    }
    
    BOOL portrait = UIInterfaceOrientationIsPortrait([UIApplication sharedApplication].statusBarOrientation);
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:self];
    navigationController.modalPresentationStyle = UIModalPresentationNone;
    _popover = [[UIPopoverController alloc] initWithContentViewController:navigationController];
    _popover.delegate = self;
    _popover.popoverContentSize = CGSizeMake(kCompletionsWindowWidth, portrait ? kCompletionsWindowWidth : 320);
    [_popover presentPopoverFromRect:rect inView:view permittedArrowDirections:UIPopoverArrowDirectionLeft | UIPopoverArrowDirectionRight animated:YES];
    
    if (showLoading)
    {
        [UIView animateWithDuration:kFadeDuration animations:^{
            _loadingLabel.alpha = 1;
            _tableView.alpha = kFadeAlpha;
        }];
        [_loadingSpinner startAnimating];
        _tableView.userInteractionEnabled = NO;
    }
}

- (void)dismissAnimated:(BOOL)animated
{
    _tableView.dataSource = nil;
    _tableView.delegate = nil;
    
    if ([_popover isPopoverVisible])
    {
        [_popover dismissPopoverAnimated:animated];
    }
}

- (void)closePopup:(id)sender
{
    [self dismissAnimated:YES];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:CompletionsPopupDidDismissNotification object:self];
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = NSLocalizedString(@"Completions", nil);
    
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Cancel"]
                                                                              style:UIBarButtonItemStylePlain
                                                                             target:self
                                                                             action:@selector(closePopup:)];
    
    _loadingLabel.text = NSLocalizedString(@"Searching...", nil);
    _loadingLabel.alpha = 0;
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    BOOL showLoading = [_completions count] == 0 || _completions.firstObject == kLoadingCompletion;
    return showLoading ? 0 : [_completions count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCompletionCellIdentifier];
    if (indexPath.row < [_completions count])
    {
        UIFont *codeFont = [[Settings instance].codeFont.regularFont fontWithSize:14];
        
        Completion *completion = [_completions objectAtIndex:indexPath.row];
        if (completion != kLoadingCompletion)
        {
            cell.textLabel.text = @"";
            if (completion.name)
            {
                NSMutableAttributedString *name = [[NSMutableAttributedString alloc] initWithString:completion.name
                                                                                         attributes:@{ NSFontAttributeName : codeFont,
                                                                                                       NSForegroundColorAttributeName : [UIColor blueColor]}];
                if (completion.completion)
                {
                    NSRange range = [completion.name rangeOfString:completion.completion];
                    if (range.location != NSNotFound)
                    {
                        [name addAttributes:@{ NSForegroundColorAttributeName : [UIColor blackColor] } range:range];
                    }
                }
                cell.textLabel.attributedText = name;
                cell.textLabel.font = codeFont;
            }
            cell.detailTextLabel.text = completion.type ? completion.type : @"";
            cell.accessoryType = completion.docString.length > 0 ? UITableViewCellAccessoryDetailDisclosureButton : UITableViewCellAccessoryNone;
        }
    }
    return cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    Completion *completion = [_completions objectAtIndex:indexPath.row];
    [[NSNotificationCenter defaultCenter] postNotificationName:CompletionsPopupDidSelectCompletionNotification object:completion];
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row < [_completions count])
    {
        Completion *completion = [_completions objectAtIndex:indexPath.row];
        if (completion.docString)
        {
            DocsViewController *docsController = [self.storyboard instantiateViewControllerWithIdentifier:NSStringFromClass([DocsViewController class])];
            docsController.completion = completion;
            UINavigationController *navigationController = (UINavigationController *)_popover.contentViewController;
            self.navigationItem.backBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:nil action:NULL];
            [navigationController pushViewController:docsController animated:YES];
        }
    }
}

#pragma mark - UIPopoverControllerDelegate

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    [[NSNotificationCenter defaultCenter] postNotificationName:CompletionsPopupDidDismissNotification object:self];
}

@end
