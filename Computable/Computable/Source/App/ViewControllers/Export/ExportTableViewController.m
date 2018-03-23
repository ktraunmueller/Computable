//
//  ExportTableViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 30.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "ExportTableViewController.h"

NSString *const ExportTableViewControllerDidSelectItemNotification = @"ExportTableViewControllerDidSelectItemNotification";
NSString *const ExportTableViewControllerSelectedItemKey = @"selection";

static NSString *const ExportTableCellIdentifier = @"ExportCell";

@implementation ExportTableViewController

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return ExportItemNumberOfItems;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:ExportTableCellIdentifier];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ExportTableCellIdentifier];
    }
//    cell.textLabel.font = [UIFont systemFontOfSize:15];
    switch (indexPath.row)
    {
//        case ExportItemPreviewPDF:
//            cell.textLabel.text = NSLocalizedString(@"Preview PDF...", nil);
//            break;
        case ExportItemSendNotebook:
            cell.textLabel.text = NSLocalizedString(@"Email Notebook...", nil);
            break;
        default:
            break;
    }
    return cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:ExportTableViewControllerDidSelectItemNotification
                                                            object:self
                                                          userInfo:@{ ExportTableViewControllerSelectedItemKey : @(indexPath.row) }];
    });
}

@end
