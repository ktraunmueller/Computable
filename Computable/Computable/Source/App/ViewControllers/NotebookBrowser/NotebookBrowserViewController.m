//
//  NotebookBrowserViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookBrowserViewController.h"
#import "NotebookBrowserCell.h"
#import "NotebookBrowserSectionView.h"
#import "NotebookBrowserLayout.h"
#import "../../AppDelegate.h"
#import "../../NSObject+App.h"
#import "../../Core/Analytics/Analytics.h"
#import "../../Core/IPython/IPythonApplication.h"
#import "../../Core/IPython/Notebook/Notebook.h"
#import "../../../Common/Utilities/UIAlertView+Blocks.h"
#import "../../../Common/Utilities/UIImage+Coloring.h"
#import "../../../Common/Utilities/UIImage+Resizing.h"

NSString *const NotebookBrowserDidSelectNotebookNotification = @"NotebookBrowserDidSelectNotebookNotification";
NSString *const NotebookBrowserDidRequestNewNotebookNotification = @"NotebookBrowserDidRequestNewNotebookNotification";
NSString *const NotebookBrowserWillDeleteNotebookNotification = @"NotebookBrowserWillDeleteNotebookNotification";

@interface NotebookBrowserViewController () <UICollectionViewDataSource, UICollectionViewDelegate, UITextFieldDelegate>
@end

@implementation NotebookBrowserViewController
{
    NSCache *_thumbnailCache;
    UIImage *_plusIcon;
    UIImage *_deleteIcon;
    UITapGestureRecognizer *_stopDeleteRecognizer;
    BOOL _wiggling;
    BOOL _cellDidRequestDelete;
}

#pragma mark - Statics

+ (NotebookBrowserViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([NotebookBrowserViewController class]);
    NotebookBrowserViewController *notebookBrowser = [storyboard instantiateViewControllerWithIdentifier:identifier];
    return notebookBrowser;
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Notebook Browser"];
    
    self.title = NSLocalizedString(@"Notebooks", nil);
    
    UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(done:)];
    self.navigationItem.rightBarButtonItem = doneButton;
    
    _thumbnailCache = [[NSCache alloc] init];
    _collectionView.dataSource = self;
    _collectionView.delegate = self;
    
    _plusIcon = [[UIImage imageNamed:@"Plus"] colorWith:[UIColor colorWithWhite:0.82 alpha:1.0]];
    _deleteIcon = [[UIImage imageNamed:@"Delete"] colorWith:[UIColor colorWithWhite:0.82 alpha:1.0]];
    
    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    [self.view addGestureRecognizer:longPressRecognizer];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [IPythonApplication reloadNotebookFiles];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(cellDidRequestDelete:)
                                                 name:NotebookBrowserCellDidRequestDeleteNotification
                                               object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - NotebookBrowserViewController

- (void)done:(id)sender
{
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

- (void)longPress:(UIGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateBegan)
    {
        _wiggling = YES;
        _stopDeleteRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapped:)];
        [self.view addGestureRecognizer:_stopDeleteRecognizer];
        for (NotebookBrowserCell *cell in _collectionView.visibleCells)
        {
            NSString *category = [IPythonApplication categoryForNotebook:cell.notebookFile];
            if ([IPythonApplication allowEditingCategory:category])
            {
                [cell startWiggling];
            }
        }
    }
}

- (void)tapped:(UIGestureRecognizer *)recognizer
{
    if (!_wiggling)
    {
        return;
    }
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        double delayInSeconds = 0.1;
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            [self stopDeletingIfNoCellDidRequestDelete];
        });
    }
}

- (void)stopDeletingIfNoCellDidRequestDelete
{
    if (_cellDidRequestDelete)
    {
        _cellDidRequestDelete = NO;
        return;
    }
    
    [self stopDeleting];
}

- (void)stopDeleting
{
    [self.view removeGestureRecognizer:_stopDeleteRecognizer];
    _stopDeleteRecognizer = nil;
    _wiggling = NO;
    for (NotebookBrowserCell *cell in _collectionView.visibleCells)
    {
        [cell stopWiggling];
    }
}

#pragma mark - Notifications

- (void)cellDidRequestDelete:(NSNotification *)notification
{
    NotebookBrowserCell *cell = notification.object;
    if ([cell.titleField isFirstResponder])
    {
        [cell.titleField resignFirstResponder]; // end editing
    }
    
    NSString *title = NSLocalizedString(@"Please confirm", nil);
    NSString *message = [NSString stringWithFormat:NSLocalizedString(@"Do you really want to delete the notebook '%@'?", nil),
                         [[cell.notebookFile lastPathComponent] stringByDeletingPathExtension]];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:NSLocalizedString(@"Delete", nil)
                                          otherButtonTitles:NSLocalizedString(@"Cancel", nil), nil];
    [alert showWithCompletion:^(UIAlertView *alertView, NSInteger buttonIndex) {
        if (buttonIndex == 1)
        {
            [self stopDeleting];
            return; // Cancel
        }
        
        _cellDidRequestDelete = YES;
        
        [[NSNotificationCenter defaultCenter] postNotificationName:NotebookBrowserWillDeleteNotebookNotification object:cell.notebookFile];
        
        if ([IPythonApplication deleteNotebook:cell.notebookFile])
        {
            NSIndexPath *indexPath = [_collectionView indexPathForCell:cell];
            [_collectionView deleteItemsAtIndexPaths:@[ indexPath ] ];
        }
        NSString *myNotebooksCategory = [[IPythonApplication notebookCategories] firstObject];
        NSArray *myNotebooks = [IPythonApplication notebooksInCategory:myNotebooksCategory];
        if ([myNotebooks count] == 0)
        {
            [self stopDeleting];
        }
    }];
}

#pragma mark - UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return [[IPythonApplication notebookCategories] count];
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView
           viewForSupplementaryElementOfKind:(NSString *)kind
                                 atIndexPath:(NSIndexPath *)indexPath
{
    if ([kind isEqualToString:UICollectionElementKindSectionHeader])
    {
        NSString *reuseIdentifier = NSStringFromClass([NotebookBrowserSectionView class]);
        NotebookBrowserSectionView *sectionView = [collectionView dequeueReusableSupplementaryViewOfKind:kind
                                                                                     withReuseIdentifier:reuseIdentifier
                                                                                            forIndexPath:indexPath];
        [sectionView.titleLabel sizeToFit];
        sectionView.separatorView.backgroundColor = [UIColor lightGrayColor];
        sectionView.separatorHeightConstraint.constant = 1 / [UIScreen mainScreen].scale;
        
        NSArray *categories = [IPythonApplication notebookCategories];
        sectionView.titleLabel.text = [categories objectAtIndex:indexPath.section];
        
        return sectionView;
    }
    return nil;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    NSString *category = [[IPythonApplication notebookCategories] objectAtIndex:section];
    NSArray *notebooks = [IPythonApplication notebooksInCategory:category];
    BOOL allowEditing = [IPythonApplication allowEditingCategory:category];
    return [notebooks count] + (allowEditing ? 1 : 0);
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    NSString *category = [[IPythonApplication notebookCategories] objectAtIndex:indexPath.section];
    NSArray *notebooks = [IPythonApplication notebooksInCategory:category];
    BOOL allowEditing = [IPythonApplication allowEditingCategory:category];
    
    NSString *reuseIdentifier = NSStringFromClass([NotebookBrowserCell class]);
    NotebookBrowserCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier
                                                                          forIndexPath:indexPath];
    cell.backgroundColor = [UIColor whiteColor];
    cell.thumbnailView.layer.borderWidth = 0.5;
    cell.thumbnailView.layer.borderColor = [[UIColor lightGrayColor] CGColor];
    
    if (indexPath.item < [notebooks count])
    {
        cell.titleField.enabled = YES;
        cell.titleField.delegate = self;
        cell.thumbnailView.contentMode = UIViewContentModeScaleAspectFit;
        cell.thumbnailView.backgroundColor = [UIColor colorWithWhite:0.98 alpha:1.0];
        cell.thumbnailView.image = nil;
        cell.thumbnailView.clipsToBounds = YES;
        cell.deleteButton.image = _deleteIcon;
        cell.deleteButton.alpha = _wiggling && allowEditing ? 1 : 0;
        cell.deleteButtonBackgroundView.alpha = cell.deleteButton.alpha;
        cell.lockIcon.hidden = YES;
        [cell ignoreWiggling:!allowEditing];
        
        NSString *notebookFile = [notebooks objectAtIndex:indexPath.item];
        cell.notebookFile = notebookFile;
        cell.titleField.text = [notebookFile stringByDeletingPathExtension];
        NSString *thumbnailPath = [IPythonApplication thumbnailPathForNotebook:notebookFile];
        UIImage *thumbnail = [_thumbnailCache objectForKey:thumbnailPath];
        if (thumbnail)
        {
            cell.thumbnailView.image = thumbnail;
        }
        else
        {
            [self loadThumbnailForCell:cell];
        }
    }
    else
    {
        BOOL fullVersionPurchased = self.appDelegate.isFullVersionPurchased;
        cell.thumbnailView.backgroundColor = [UIColor whiteColor];
        cell.thumbnailView.layer.borderColor = [[UIColor colorWithWhite:0.82 alpha:1.0] CGColor];
        cell.thumbnailView.contentMode = UIViewContentModeCenter;
        cell.thumbnailView.image = _plusIcon;
        cell.titleField.text = @"";
        cell.titleField.enabled = NO;
        cell.deleteButton.image = _deleteIcon;
        cell.deleteButton.hidden = YES;
        cell.deleteButtonBackgroundView.hidden = cell.deleteButton.hidden;
        cell.lockIcon.hidden = fullVersionPurchased;
        cell.lockIcon.alpha = 0.5;
        [cell ignoreWiggling:YES];
    }
    if (_wiggling && allowEditing)
    {
        [cell startWiggling];
    }
    
    return cell;
}

- (void)loadThumbnailForCell:(NotebookBrowserCell *)cell
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSString *thumbnailPath = [IPythonApplication thumbnailPathForNotebook:cell.notebookFile];
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:thumbnailPath isDirectory:NULL])
        {
            return;
        }
        UIImage *thumbnail = [UIImage imageWithContentsOfFile:thumbnailPath];
        if (thumbnail == nil)
        {
            return;
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [_thumbnailCache setObject:thumbnail forKey:thumbnailPath];
            cell.thumbnailView.image = thumbnail;
        });
    });
}

#pragma mark - UICollectionViewDelegate

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    [collectionView deselectItemAtIndexPath:indexPath animated:YES];
    
    NSArray *categories = [IPythonApplication notebookCategories];
    NSArray *notebooks = [IPythonApplication notebooksInCategory:[categories objectAtIndex:indexPath.section]];
    if (indexPath.item < [notebooks count])
    {
        NSString *notebookPath = [notebooks objectAtIndex:indexPath.item];
        [[NSNotificationCenter defaultCenter] postNotificationName:NotebookBrowserDidSelectNotebookNotification object:notebookPath];
    }
    else
    {
        [[NSNotificationCenter defaultCenter] postNotificationName:NotebookBrowserDidRequestNewNotebookNotification object:nil];
    }
}

#pragma mark - UICollectionViewDelegateFlowLayout

#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    NotebookBrowserCell *cell = (NotebookBrowserCell *)textField.superview.superview;
    NSString *category = [IPythonApplication categoryForNotebook:cell.notebookFile];
    return [IPythonApplication allowEditingCategory:category];
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    NotebookBrowserCell *cell = (NotebookBrowserCell *)textField.superview.superview;
    NSString *originalNotebookFile = cell.notebookFile;
    NSString *category = [IPythonApplication categoryForNotebook:cell.notebookFile];
    NSIndexPath *oldIndexPath = [_collectionView indexPathForCell:cell];
    cell.notebookFile = [IPythonApplication renameNotebookFromFile:originalNotebookFile toFile:textField.text];
    NSUInteger newIndex = [[IPythonApplication notebooksInCategory:category] indexOfObject:cell.notebookFile];
    if (oldIndexPath && newIndex != NSNotFound && newIndex != oldIndexPath.item)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            [_collectionView moveItemAtIndexPath:oldIndexPath toIndexPath:[NSIndexPath indexPathForItem:newIndex inSection:oldIndexPath.section]];
        });
    }
    return YES;
}

@end
