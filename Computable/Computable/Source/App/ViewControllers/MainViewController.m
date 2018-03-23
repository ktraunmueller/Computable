//
//  ViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "MainViewController.h"
#import "Dropbox/DropboxBrowserViewController.h"
#import "Export/ExportTableViewController.h"
#import "Notebook/NotebookViewController.h"
#import "Notebook/InputCellView.h"
#import "NotebookBrowser/NotebookBrowserViewController.h"
#import "Settings/SettingsViewController.h"
#import "Tabs/TabsViewController.h"
#import "Tabs/TabView.h"
#import "../AppDelegate.h"
#import "../NSObject+App.h"
#import "../Colors.h"
#import "../Core/Analytics/Analytics.h"
#import "../Core/Dropbox/Dropbox.h"
#import "../Core/Dropbox/DropboxSync.h"
#import "../Core/InAppPurchase/InAppPurchase.h"
#import "../Core/IPython/IPythonApplication.h"
#import "../Core/IPython/Notebook/Notebook.h"
#import "../Core/IPython/Notebook/Cells/InputCell.h"
#import "../Core/Markdown/MarkdownHtmlRenderer.h"
#import "../Core/Migrations/Migrations.h"
#import "../Core/Python/Runtime/PythonRuntime.h"
#import "../Tour/TourPage.h"
#import "../Tour/TourIntroViewController.h"
#import "../Tour/TourStageViewController.h"
#import "../Views/Html/HtmlCaptureView.h"
#import "../../Common/Utilities/UIAlertView+Blocks.h"
#import "../../Common/Utilities/UIImage+Overlay.h"
#import "../../Common/Utilities/UIViewController+Containment.h"
#import "../../Common/Views/HatchedProgressView.h"
#import "../../Common/ViewControllers/AutorotatingNavigationController.h"

#import <Dropbox/Dropbox.h>
#import <MessageUI/MessageUI.h>

static const NSUInteger kMaxmiumNumberOfOpenNotebooks = 10;

static const NSTimeInterval kPanelSlideAnimationDuration = 0.3;
static const NSTimeInterval kLabelSlideAnimationDuration = 0.15;

static NSString *const kDidShowTourDefaultsKey = @"DidShowNewTour";

@interface MainViewController () <UIDocumentInteractionControllerDelegate, MFMailComposeViewControllerDelegate>
@end

@implementation MainViewController
{
    BOOL _statusPanelVisible;
    BOOL _didRunLaunchSequence;
    BOOL _didLaunchIPython;
    UIBarButtonItem *_newNotebookItem;
    UIBarButtonItem *_dropboxItem;
    UIBarButtonItem *_exportItem;
    UIBarButtonItem *_libraryItem;
    UIBarButtonItem *_settingsItem;
    TabsViewController *_tabsController;
    HtmlCaptureView *_htmlCaptureView;
    HatchedProgressView *_statusProgressView;
    BOOL _hideStatusBar;
    CGPoint _statusLabelCenter;
    CGPoint _statusLabelCenterAbove;
    CGPoint _statusLabelCenterBelow;
    UIPopoverController *_exportPopover;
    UIDocumentInteractionController *_previewController;
    BOOL _showingMemoryAlert;
    TourIntroViewController *_tourIntroViewController;
    TourStageViewController *_tourViewController;
}

#pragma mark - MainViewController

- (void)setupNewNotebookButton
{
    UIImage *plusImage = [UIImage imageNamed:@"Plus"];
    if (!self.appDelegate.isFullVersionPurchased)
    {
        UIImage *lockImage = [UIImage imageNamed:@"Lock"];
        plusImage = [plusImage imageWithOverlay:lockImage atFrame:CGRectMake(11, 0, 12, 12)];
    }
    _newNotebookItem =  [[UIBarButtonItem alloc] initWithImage:plusImage
                                                         style:UIBarButtonItemStylePlain
                                                        target:self
                                                        action:@selector(newNotebook:)];
    self.navigationItem.leftBarButtonItem = _newNotebookItem;
}

- (void)endEditingActiveNotebook
{
    if (_tabsController.activeTab)
    {
        NotebookViewController *notebookController = (NotebookViewController *)_tabsController.activeTab.contentController;
        [notebookController endEditing];
    }
}

- (void)newNotebook:(id)sender
{
    if (!self.appDelegate.isFullVersionPurchased)
    {
        [self showPurchaseDialog];
        return;
    }
    [self endEditingActiveNotebook];
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"new" label:nil value:nil];
    
    NSString *message = NSLocalizedString(@"Creating new notebook...", nil);
    [self showStatusPanelWithText:message completion:^{
        [IPythonApplication newNotebook:^(Notebook *notebook) {
            [self hideStatusPanel:nil];
            NotebookViewController *notebookController = [NotebookViewController newInstanceWithNotebook:notebook storyboard:self.storyboard];
            notebookController.htmlCaptureView = _htmlCaptureView;
            [_tabsController addTabController:notebookController];
        }];
    }];
}

- (void)showLibrary:(id)sender
{
    [self endEditingActiveNotebook];
    
    NotebookBrowserViewController *notebookBrowser = [NotebookBrowserViewController newInstanceFromStoryboard:self.storyboard];
    AutorotatingNavigationController *navigationController = [[AutorotatingNavigationController alloc] initWithRootViewController:notebookBrowser];
    [self presentViewController:navigationController animated:YES completion:nil];
}

- (void)browseDropbox:(id)sender
{
    NotebookViewController *notebookController = (NotebookViewController *)_tabsController.activeTab.contentController;
    [notebookController markSelectedRange];
    
    DropboxBrowserViewController *dropboxBrowser = [DropboxBrowserViewController newInstanceFromStoryboard:self.storyboard];
    dropboxBrowser.singleSelection = YES;
    dropboxBrowser.suppressedExtensions = @[ @"py", @"ipynb" ];
    dropboxBrowser.completion = ^(NSArray *selectedFiles) {
        DBFileInfo *file = [selectedFiles firstObject];
        if (file)
        {
            NSString *dropboxPath = file.path.stringValue;
            if ([Dropbox fileExistsLocally:dropboxPath])
            {
                [notebookController restoreSelectedRange];
                [notebookController fileSelected:[Dropbox fileSystemPath:dropboxPath]];
                return;
            }
            [self showStatusPanelWithText:@"Downloading file..." completion:^{
                [[DropboxSync instance] cacheAndRegisterFile:dropboxPath completion:^(NSString *localPath) {
                    [self hideStatusPanel:^{
                        [notebookController restoreSelectedRange];
                        [notebookController fileSelected:localPath];
                    }];
                }];
            }];
        }
    };
    AutorotatingNavigationController *navigationController = [[AutorotatingNavigationController alloc] initWithRootViewController:dropboxBrowser];
    navigationController.modalPresentationStyle = UIModalPresentationFormSheet;
    [self presentViewController:navigationController animated:YES completion:nil];
}

- (void)export:(id)sender
{
    if ([_exportPopover isPopoverVisible])
    {
        [_exportPopover dismissPopoverAnimated:YES];
        return;
    }
    
    TabView *tab = _tabsController.activeTab;
    if (tab == nil)
    {
        return;
    }
    
    [self endEditingActiveNotebook];
    
    ExportTableViewController *exportController = [[ExportTableViewController alloc] initWithStyle:UITableViewStylePlain];
    _exportPopover = [[UIPopoverController alloc] initWithContentViewController:exportController];
    _exportPopover.popoverContentSize = CGSizeMake(320, 44 * ExportItemNumberOfItems);
    [_exportPopover presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
}

- (void)previewPDF
{
    [_exportPopover dismissPopoverAnimated:YES];
    
    TabView *tab = _tabsController.activeTab;
    if (tab == nil)
    {
        return;
    }
//    NotebookViewController *notebookController = (NotebookViewController *)tab.contentController;
//    NSString *pdfPath = [notebookController savePDF];
//    NSURL *pdfURL = [NSURL fileURLWithPath:pdfPath];
//    _previewController = [UIDocumentInteractionController interactionControllerWithURL:pdfURL];
//    _previewController.delegate = self;
//    if ([_previewController presentPreviewAnimated:NO])
//    {
//        // nothing to do - quick look preview available
//    }
//    else
//    {
//        [_previewController presentOptionsMenuFromBarButtonItem:_exportItem animated:YES];
//    }
}

- (void)sendNotebook
{
    [_exportPopover dismissPopoverAnimated:YES];
    
    [self endEditingActiveNotebook];
    
    if (![MFMailComposeViewController canSendMail])
    {
        NSString *title = NSLocalizedString(@"Cannot send mail", nil);
        NSString *message = NSLocalizedString(@"It looks like you don't have any email accounts set up yet. Please check your email settings.", nil);
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"Ok", nil)
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    TabView *tab = _tabsController.activeTab;
    if (tab == nil)
    {
        return;
    }
    NotebookViewController *notebookController = (NotebookViewController *)tab.contentController;
    [notebookController.notebook save:YES];
    NSString *path = notebookController.notebook.path;
    NSData *data = [NSData dataWithContentsOfFile:path];
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"export.email" label:[path lastPathComponent] value:nil];
    
    MFMailComposeViewController *mailComposeViewController = [[MFMailComposeViewController alloc] init];
    mailComposeViewController.mailComposeDelegate = self;
    [mailComposeViewController setSubject:[path lastPathComponent]];
    [mailComposeViewController addAttachmentData:data mimeType:@"application/json" fileName:[path lastPathComponent]];
    [self presentViewController:mailComposeViewController animated:YES completion:nil];
}

- (void)showSettings:(id)sender
{
    [self endEditingActiveNotebook];
    
    SettingsViewController *settingsController = [SettingsViewController newInstanceFromStoryboard:self.storyboard];
    AutorotatingNavigationController *navigationController = [[AutorotatingNavigationController alloc] initWithRootViewController:settingsController];
    navigationController.modalPresentationStyle = UIModalPresentationFormSheet;
    [self presentViewController:navigationController animated:YES completion:nil];
}

- (void)hideStatusBar:(BOOL)hide
{
    _hideStatusBar = hide;
    [self setNeedsStatusBarAppearanceUpdate];
}

//- (void)hideNavigationBar:(BOOL)hide animated:(BOOL)animated
//{
//    [self.navigationController setNavigationBarHidden:hide animated:animated];
//}

- (void)showStatusPanelWithText:(NSString *)status completion:(dispatch_block_t)completion
{
    if (_statusPanelVisible)
    {
        return;
    }
    _statusPanelVisible = YES;
    
    [self.view bringSubviewToFront:_statusPanelView];
    [_statusProgressView startAnimating];
    _statusLabel.text = status;
    _statusPanelTopConstraint.constant = -16;
    [UIView animateWithDuration:kPanelSlideAnimationDuration animations:^{
        [self.view layoutIfNeeded];
    } completion:^(BOOL finished) {
        if (completion)
        {
            completion();
        }
    }];
}

- (void)hideStatusPanel:(dispatch_block_t)completion
{
    if (!_statusPanelVisible)
    {
        return;
    }
    _statusPanelVisible = NO;
    
    _statusPanelTopConstraint.constant = -50;
    [UIView animateWithDuration:kPanelSlideAnimationDuration animations:^{
        [self.view layoutIfNeeded];
    } completion:^(BOOL finished) {
        if (!finished)
        {
            return;
        }
        [_statusProgressView stopAnimating];
        if (completion)
        {
            completion();
        }
    }];
}

- (void)updateStatusPanelWithText:(NSString *)status completion:(dispatch_block_t)completion
{
    [UIView animateWithDuration:kLabelSlideAnimationDuration animations:^{
        _statusLabel.center = _statusLabelCenterAbove;
    } completion:^(BOOL finished) {
        if (!finished)
        {
            return;
        }
        _statusLabel.text = status;
        _statusLabel.center = _statusLabelCenterBelow;
        [UIView animateWithDuration:kLabelSlideAnimationDuration
                         animations:^{
                             _statusLabel.center = _statusLabelCenter;
                         } completion:^(BOOL finished) {
                             if (!finished)
                             {
                                 return;
                             }
                             if (completion)
                             {
                                 completion();
                             }
                         }];
    }];
}

- (void)showOrUpdateStatusPanelWithText:(NSString *)status completion:(dispatch_block_t)completion
{
    if (_statusPanelVisible)
    {
        [self updateStatusPanelWithText:status completion:completion];
    }
    else
    {
        [self showStatusPanelWithText:status completion:completion];
    }
}

- (void)showPurchaseDialog
{
    [Analytics trackEventWithCategory:@"Purchase" action:@"show" label:nil value:nil];
    
    NSString *price = self.appDelegate.fullVersionLocalizedPrice;
    NSString *priceInfo = price ? [NSString stringWithFormat:NSLocalizedString(@" for %@", nil), price] : nil;
    NSString *title = NSLocalizedString(@"Unlock all Features", nil);
    NSString *message = [NSString stringWithFormat:NSLocalizedString(@"Creating new notebooks is not available in the free version. Click the Purchase button to unlock all features now%@.", nil), priceInfo ? priceInfo : @""];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                          otherButtonTitles:NSLocalizedString(@"Purchase", nil), nil];
    [alert showWithCompletion:^(UIAlertView *alertView, NSInteger buttonIndex) {
        switch (buttonIndex)
        {
            case 0: // Cancel
                [Analytics trackEventWithCategory:@"Purchase" action:@"cancel" label:nil value:nil];
                break;
            case 1:
            {
                [self dismissOpenModalViewControllerWithCompletion:^{
                    [Analytics trackEventWithCategory:@"Purchase" action:@"purchase" label:nil value:nil];
                    [self.appDelegate purchaseFullVersion];
                }];
            }
                break;
            default:
                break;
        }
    }];
}

- (void)dismissOpenModalViewControllerWithCompletion:(dispatch_block_t)completion
{
    if (self.presentedViewController)
    {
        [self dismissViewControllerAnimated:YES completion:completion];
        return;
    }
    if (completion)
    {
        completion();
    }
}

- (void)openNotebook:(NSString *)notebookPath
{
    for (TabView *tab in _tabsController.tabs)
    {
        NotebookViewController *notebookController = (NotebookViewController *)tab.contentController;
        if ([[notebookController.notebook.path lastPathComponent] isEqualToString:[notebookPath lastPathComponent]])
        {
            [_tabsController activateTab:tab];
            return;
        }
    }
    NSString *message = [NSString stringWithFormat:NSLocalizedString(@"Loading '%@'...", nil),
                         [notebookPath.lastPathComponent stringByDeletingPathExtension]];
    [self showStatusPanelWithText:message completion:^{
        [IPythonApplication openNotebook:notebookPath completion:^(Notebook *notebook) {
            [notebook loadWithOutputCells:YES];
            [self hideStatusPanel:nil];
            NotebookViewController *notebookController = [NotebookViewController newInstanceWithNotebook:notebook storyboard:self.storyboard];
            notebookController.htmlCaptureView = _htmlCaptureView;
            [_tabsController addTabController:notebookController];
        }];
    }];
}

- (void)closeAllNotebooksExceptActiveOne
{
    if ([_tabsController.tabs count] <= 1)
    {
        return;
    }
    TabView *activTab = _tabsController.activeTab;
    NSArray *tabs = [_tabsController.tabs copy];
    for (TabView *tab in tabs)
    {
        if (tab == activTab)
        {
            continue;
        }
        [_tabsController closeTab:tab];
    }
}

- (void)startLaunchSequence
{
    if (_didRunLaunchSequence)
    {
        return;
    }
    _didRunLaunchSequence = YES;
    
    PythonRuntime *runtime = [PythonRuntime instance];
    if (runtime.needsRuntimeFolderSetup)
    {
        [self showStatusPanelWithText:NSLocalizedString(@"Building file cache...", nil) completion:^{
            [runtime setupRuntimeFolder:^{
                [self runMigrationsAndLaunchIPython];
            }];
            
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Information", nil)
                                                                message:NSLocalizedString(@"This somewhat lengthy cache setup is performed only once, to help speed up subsequent launches.", nil)
                                                               delegate:nil
                                                      cancelButtonTitle:nil
                                                      otherButtonTitles:NSLocalizedString(@"OK", nil), nil];
                [alert show];
            });
        }];
        return;
    }
    [self runMigrationsAndLaunchIPython];
}

- (void)runMigrationsAndLaunchIPython
{
    dispatch_block_t launchIPython = ^{
        [self showOrUpdateStatusPanelWithText:NSLocalizedString(@"Starting Python runtime...", nil) completion:^{
            [[PythonRuntime instance] launch:^{
                [self updateStatusPanelWithText:NSLocalizedString(@"Starting IPython...", nil) completion:^{
                    [IPythonApplication start:^{
                        [self hideStatusPanel:nil];
                        _didLaunchIPython = YES;
                        
                        [self.appDelegate runPostLaunchActions];
                    }];
                }];
            }];
        }];
    };
    
    [IPythonApplication prepare:^{
        if ([Migrations hasPendingMigrations])
        {
            [self showOrUpdateStatusPanelWithText:NSLocalizedString(@"Updating files...", nil) completion:^{
                [Migrations runPendingMigrations:^{
                    launchIPython();
                }];
            }];
            return;
        }
        launchIPython();
    }];
}

- (BOOL)shouldShowTour
{
    return ![[NSUserDefaults standardUserDefaults] boolForKey:kDidShowTourDefaultsKey];
}

- (void)markTourShown
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults setBool:YES forKey:kDidShowTourDefaultsKey];
    [userDefaults synchronize];
}

- (void)showTour
{
    _tourIntroViewController = [TourIntroViewController newInstance];
    __weak typeof(self) weakSelf = self;
    _tourIntroViewController.startTourBlock = ^{
        [weakSelf startTour];
    };
    _tourIntroViewController.skipTourBlock = ^{
        [weakSelf closeTour];
    };
    [self installChildViewController:_tourIntroViewController inView:self.view];
}

- (void)startTour
{
    if (_tourIntroViewController)
    {
        [self uninstallChildViewController:_tourIntroViewController];
        _tourIntroViewController = nil;
    }
    _tourViewController = [TourStageViewController newInstance];
    [self installChildViewController:_tourViewController inView:self.view];
}

- (void)closeTour
{
    [self markTourShown];
    
    if (_tourIntroViewController)
    {
        [self uninstallChildViewController:_tourIntroViewController];
        _tourIntroViewController = nil;
    }
    if (_tourViewController)
    {
        [self uninstallChildViewController:_tourViewController];
        _tourViewController = nil;
    }
    
    [self startLaunchSequence];
}

#pragma mark - Notifications

- (void)notebookApplicationDidStart:(NSNotification *)notification
{
    _newNotebookItem.enabled = YES;
    _libraryItem.enabled = YES;
}

- (void)notebookApplicationDidStop:(NSNotification *)notification
{
    _newNotebookItem.enabled = NO;
    _libraryItem.enabled = NO;
}

- (void)selectedTabDidChange:(NSNotification *)notification
{
    _dropboxItem.enabled = NO;
    _exportItem.enabled = [_tabsController.tabs count] > 0;
    _libraryItem.enabled = [_tabsController.tabs count] < kMaxmiumNumberOfOpenNotebooks;
    _newNotebookItem.enabled = [_tabsController.tabs count] < kMaxmiumNumberOfOpenNotebooks;
}

- (void)cellDidBeginEditing:(NSNotification *)notification
{
    InputCell *inputCell = notification.object;
    _dropboxItem.enabled = inputCell.type == InputCellTypeCode && [Dropbox isLinked];
}

- (void)cellDidEndEditing:(NSNotification *)notification
{
    _dropboxItem.enabled = NO;
}

- (void)notebookBrowserDidSelectNotebook:(NSNotification *)notification
{
    [self dismissViewControllerAnimated:YES completion:nil];
    
    NSString *notebookPath = notification.object;
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"open.browser" label:[notebookPath lastPathComponent] value:nil];
    
    [self openNotebook:notebookPath];
}

- (void)notebookBrowserDidRequestNewNotebook:(NSNotification *)notification
{
    if (!self.appDelegate.isFullVersionPurchased)
    {
        [self showPurchaseDialog];
        return;
    }
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"new.browser" label:nil value:nil];
    
    [self dismissViewControllerAnimated:YES completion:^{
        [self newNotebook:nil];
    }];
}

- (void)notebookBrowserWillDeleteNotebook:(NSNotification *)notification
{
    NSString *notebookPath = notification.object;
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"delete.browser" label:[notebookPath lastPathComponent] value:nil];
    
    for (TabView *tab in _tabsController.tabs)
    {
        NotebookViewController *notebookController = (NotebookViewController *)tab.contentController;
        if ([[notebookController.notebook.path lastPathComponent] isEqualToString:[notebookPath lastPathComponent]])
        {
            [_tabsController closeTab:tab];
            return;
        }
    }
}

- (void)showTourRequested:(NSNotification *)notification
{
    [self startTour];
}

- (void)closeTourRequested:(NSNotification *)notification
{
    [self closeTour];
}

- (void)exportControllerDidSelectItem:(NSNotification *)notification
{
    ExportItem item = (ExportItem)[[notification.userInfo valueForKey:ExportTableViewControllerSelectedItemKey] integerValue];
    switch (item)
    {
            //        case ExportItemPreviewPDF:
            //            [self previewPDF];
            //            break;
        case ExportItemSendNotebook:
            [self sendNotebook];
            break;
        default:
            break;
    }
}

- (void)purchaseWillStart:(NSNotification *)notification
{
    [self showStatusPanelWithText:NSLocalizedString(@"Contacting App Store...", nil) completion:nil];
}

- (void)purchaseDidFinish:(NSNotification *)notification
{
    [self setupNewNotebookButton];
    [self hideStatusPanel:nil];
}

- (void)dropboxDownloadDidStart:(NSNotification *)notification
{
    [self showStatusPanelWithText:NSLocalizedString(@"Downloading from Dropbox...", nil) completion:nil];
}

- (void)dropboxDownloadDidStop:(NSNotification *)notification
{
    [self hideStatusPanel:nil];
}

- (void)dropboxWrite:(NSNotification *)notification
{
    if (!_statusPanelVisible)
    {
        [self showStatusPanelWithText:NSLocalizedString(@"Writing to Dropbox...", nil) completion:^{
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                [self hideStatusPanel:nil];
            });
        }];
    }
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Main"];
    
    // navigation bar
    self.title = @"Computable";
    self.edgesForExtendedLayout = UIRectEdgeNone;
    
    [self setupNewNotebookButton];
    
    _libraryItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Library"]
                                                    style:UIBarButtonItemStylePlain
                                                   target:self
                                                   action:@selector(showLibrary:)];
    _dropboxItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Download"]
                                                    style:UIBarButtonItemStylePlain
                                                   target:self
                                                   action:@selector(browseDropbox:)];
    _exportItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Export"]
                                                   style:UIBarButtonItemStylePlain
                                                  target:self
                                                  action:@selector(export:)];
    _settingsItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"Settings"]
                                                     style:UIBarButtonItemStylePlain
                                                    target:self
                                                    action:@selector(showSettings:)];
    self.navigationItem.rightBarButtonItems = @[ _settingsItem, /*_helpItem,*/ _libraryItem, _exportItem, _dropboxItem ];
    
    _newNotebookItem.enabled = NO;
    _libraryItem.enabled = NO;
    _exportItem.enabled = NO;
    _dropboxItem.enabled = NO;
    
    // HTML renderer
    _htmlCaptureView = [[HtmlCaptureView alloc] initWithBaseUrl:[MarkdownHtmlRenderer baseUrl] readyCallback:MathJaxReadyCallback];
    _htmlCaptureView.frame = CGRectMake(0, 32, CGRectGetWidth(self.view.bounds), CGRectGetHeight(self.view.bounds) - 32);
    _htmlCaptureView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    _htmlCaptureView.hidden = YES;
    [self.view addSubview:_htmlCaptureView];
    
    // tabs
    _tabsController = [TabsViewController newInstance];
    [self installChildViewController:_tabsController inView:self.view];
    
    // status panel
    _statusPanelView.backgroundColor = [UIColor whiteColor];// LightGrayBackground;
    _statusPanelView.layer.borderWidth = 1 / [UIScreen mainScreen].scale;
    _statusPanelView.layer.borderColor = [[UIColor lightGrayColor] CGColor];
    _statusPanelView.layer.cornerRadius = 5;
    _statusPanelView.clipsToBounds = YES;
    _statusProgressView = [[HatchedProgressView alloc] initWithFrame:_statusPanelView.bounds];
    [_statusPanelView addSubview:_statusProgressView];
    [_statusPanelView sendSubviewToBack:_statusProgressView];
    _statusLabel.textColor = [UIColor darkGrayColor];
    _statusLabel.text = @"";
    _statusPanelTopConstraint.constant = -50;
    [self.view setNeedsLayout];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookApplicationDidStart:)
                                                 name:NotebookApplicationDidStartNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookApplicationDidStop:)
                                                 name:NotebookApplicationDidStopNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(cellDidBeginEditing:)
                                                 name:InputCellDidBeginEditingNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(cellDidEndEditing:)
                                                 name:InputCellDidEndEditingNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(selectedTabDidChange:)
                                                 name:SelectedTabDidChangeNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookBrowserDidSelectNotebook:)
                                                 name:NotebookBrowserDidSelectNotebookNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookBrowserDidRequestNewNotebook:)
                                                 name:NotebookBrowserDidRequestNewNotebookNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookBrowserWillDeleteNotebook:)
                                                 name:NotebookBrowserWillDeleteNotebookNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(showTourRequested:)
                                                 name:ShowTourRequestedNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(closeTourRequested:)
                                                 name:CloseTourRequestedNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(exportControllerDidSelectItem:)
                                                 name:ExportTableViewControllerDidSelectItemNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(purchaseWillStart:)
                                                 name:InAppPurchaseWillStartNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(purchaseDidFinish:)
                                                 name:InAppPurchaseDidFinishNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(dropboxDownloadDidStart:)
                                                 name:DropboxDownloadDidStartNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(dropboxDownloadDidStop:)
                                                 name:DropboxDownloadDidStopNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(dropboxWrite:)
                                                 name:DropboxWriteNotification
                                               object:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([_tabsController.tabs count] <= 1)
    {
        return;
    }
    if (_showingMemoryAlert)
    {
        return;
    }
    _showingMemoryAlert = YES;
    NSString *title = NSLocalizedString(@"Low Memory", nil);
    NSString *message = NSLocalizedString(@"Computable needs to free up some memory. Close all notebooks except the current one?", nil);
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                          otherButtonTitles:NSLocalizedString(@"Close", nil), nil];
    [alert showWithCompletion:^(UIAlertView *alertView, NSInteger buttonIndex) {
        _showingMemoryAlert = NO;
        if (buttonIndex == 0)
        {
            return;
        }
        [self closeAllNotebooksExceptActiveOne];
    }];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _statusLabelCenter = _statusLabel.center;
        _statusLabelCenterAbove = _statusLabelCenter; _statusLabelCenterAbove.y -= 16;
        _statusLabelCenterBelow = _statusLabelCenter; _statusLabelCenterBelow.y += 16;
        if ([self shouldShowTour])
        {
            [self showTour];
        }
        else
        {
            [self startLaunchSequence];
        }
    });
}

- (UIStatusBarAnimation)preferredStatusBarUpdateAnimation
{
    return UIStatusBarAnimationSlide;
}

- (BOOL)prefersStatusBarHidden
{
    return _hideStatusBar;
}

#pragma mark - UIViewController (UIViewControllerRotation)

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

#pragma mark - MFMailComposeViewControllerDelegate

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
{
    [controller.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - UIDocumentInteractionControllerDelegate

- (UIViewController *)documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController *)controller
{
    return self;
}

@end
