//
//  DropboxBrowserViewController.m
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 17.07.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import "DropboxBrowserViewController.h"
#import "DropboxBrowserCell.h"
#import "../../Core/Analytics/Analytics.h"
#import "../../Core/Dropbox/Dropbox.h"

#import <Dropbox/Dropbox.h>
#import <QuartzCore/QuartzCore.h>

@interface DropboxBrowserViewController ()

@property (nonatomic, strong) DBPath *path;
@property (nonatomic, readonly) NSMutableArray *selectedFiles;

@end

@implementation DropboxBrowserViewController
{
    NSMutableArray *_pathContents;
    NSMutableDictionary *_fileExists;
    NSMutableArray *_selectedFiles;
}

#pragma mark - Statics

+ (DropboxBrowserViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([DropboxBrowserViewController class]);
    DropboxBrowserViewController *dropboxBrowserController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    return dropboxBrowserController;
}

#pragma mark - Properties

- (DropboxBrowserViewController *)rootBrowser
{
    if (self.parentBrowser)
    {
        return self.parentBrowser.rootBrowser;
    }
    return self;
}

#pragma mark - DropboxBrowserViewcontroller

- (void)dismiss:(id)sender
{
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

- (void)drillDownInto:(DBFileInfo *)fileInfo
{
    DropboxBrowserViewController *browser = [DropboxBrowserViewController newInstanceFromStoryboard:self.storyboard];
    browser.parentBrowser = self;
    browser.path = fileInfo.path;
    browser.suppressedExtensions = self.suppressedExtensions;
    browser.singleSelection = self.singleSelection;
    [self.navigationController pushViewController:browser animated:YES];
}

- (void)done:(id)sender
{
    DropboxBrowserViewController *rootBrowser = self.rootBrowser;
    if (rootBrowser.completion)
    {
        rootBrowser.completion(rootBrowser.selectedFiles);
    }
    [rootBrowser dismiss:nil];
}

- (NSString *)trimTitle:(DBPath *)path
{
    NSString *name = [path.stringValue lastPathComponent];
    const NSInteger MaxLength = 15;
    return [name length] < MaxLength ? name : [[name substringToIndex:MaxLength] stringByAppendingString:@"..."];
}

- (void)toggleSelectionForFile:(DBFileInfo *)fileInfo
{
    DropboxBrowserViewController *rootBrowser = self.rootBrowser;
    if (rootBrowser == self)
    {
        if (![_selectedFiles containsObject:fileInfo])
        {
            if (_singleSelection)
            {
                [_selectedFiles removeAllObjects];
            }
            [_selectedFiles addObject:fileInfo];
        }
        else
        {
            [_selectedFiles removeObject:fileInfo];
        }
        return;
    }
    [rootBrowser toggleSelectionForFile:fileInfo];
}

- (BOOL)isFileSelected:(DBFileInfo *)fileInfo
{
    DropboxBrowserViewController *rootBrowser = self.rootBrowser;
    if (rootBrowser == self)
    {
        return [_selectedFiles containsObject:fileInfo];
    }
    return [rootBrowser isFileSelected:fileInfo];
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Dropbox browser"];
    
    self.title = @"Dropbox";
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                           target:self
                                                                                           action:@selector(done:)];
    
    _notConnectedView.hidden = YES;
    _notConnectedHintLabel.textColor = [UIColor darkGrayColor];
    _notConnectedHintLabel.text = NSLocalizedString(@"Dropbox ist not linked to any account.\n\nPlease link your Dropbox account in the settings.", nil);
    
    _selectedFiles = [NSMutableArray array];
    
    _tableView.hidden = NO;
    [_tableView registerClass:[DropboxBrowserCell class] forCellReuseIdentifier:DropboxBrowserCellIdentifier];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 0.01)];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (![Dropbox isLinked])
    {
        _tableView.hidden = YES;
        _notConnectedView.hidden = NO;
        return;
    }
    if (self.path == nil)
    {
        self.path = [DBPath root];
    }
    
    DropboxListCompletion observer = ^(NSArray *pathContents) {
        _pathContents = [NSMutableArray array];
        _fileExists = [NSMutableDictionary dictionary];
        for (DBFileInfo *fileInfo in pathContents)
        {
            if ([_suppressedExtensions containsObject:[fileInfo.path.stringValue.pathExtension lowercaseString]])
            {
                continue;
            }
            [_pathContents addObject:fileInfo];
            BOOL fileExists = [Dropbox fileExistsLocally:fileInfo.path.stringValue];
            [_fileExists setObject:@(fileExists) forKey:fileInfo.path.stringValue];
        }
        [_tableView reloadData];
    };
    
    self.title = self.path == [DBPath root] ? @"Dropbox" : [self trimTitle:self.path];
    DBFilesystem *fileSystem = [DBFilesystem sharedFilesystem];
    [fileSystem addObserver:self forPathAndChildren:self.path block:^{
        [Dropbox listFolderContents:self.path completion:observer];
    }];
    
    [Dropbox listFolderContents:self.path completion:observer];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [[DBFilesystem sharedFilesystem] removeObserver:self];
    
    [super viewWillDisappear:animated];
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [_pathContents count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    DropboxBrowserCell *cell = [tableView dequeueReusableCellWithIdentifier:DropboxBrowserCellIdentifier forIndexPath:indexPath];
    
    cell.fileInfo = [_pathContents objectAtIndex:indexPath.row]; // TODO had an out-of-bounds crash here
    BOOL fileExists = [_fileExists[cell.fileInfo.path.stringValue] boolValue];
    BOOL fileSelected = [self isFileSelected:cell.fileInfo];
    cell.textLabel.text = [cell.fileInfo.path.stringValue lastPathComponent];
    cell.textLabel.textColor = cell.fileInfo.isFolder || fileExists ? [UIColor blackColor] : [UIColor darkGrayColor];
    cell.detailTextLabel.textColor = cell.textLabel.textColor;
    
    if (cell.fileInfo.isFolder)
    {
        cell.detailTextLabel.text = @"";
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    else
    {
        cell.detailTextLabel.text = fileExists ? [Dropbox userHomeRelativePath:cell.fileInfo.path.stringValue] : (fileSelected ? NSLocalizedString(@"Will be downloaded", nil) : @"");
        cell.accessoryType = fileSelected ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    }
    
    return cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    DBFileInfo *fileInfo = [_pathContents objectAtIndex:indexPath.row];
    if (fileInfo.isFolder)
    {
        [self drillDownInto:fileInfo];
    }
    else
    {
        NSMutableArray *indexPaths = [NSMutableArray arrayWithObject:indexPath];
        NSUInteger index = 0;
        for (DBFileInfo *fileInfo in _pathContents)
        {
            NSIndexPath *otherIndexPath = [NSIndexPath indexPathForRow:index inSection:0];
            if ([self isFileSelected:fileInfo] && ![indexPaths containsObject:otherIndexPath])
            {
                [indexPaths addObject:otherIndexPath];
            }
            index++;
        }
        [self toggleSelectionForFile:fileInfo];
        [_tableView reloadRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationAutomatic];
    }
}

@end
