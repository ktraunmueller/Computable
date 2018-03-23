//
//  ExportTableViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 30.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSUInteger, ExportItem)
{
//    ExportItemPreviewPDF,
    ExportItemSendNotebook,
    ExportItemNumberOfItems
};

extern NSString *const ExportTableViewControllerDidSelectItemNotification;
extern NSString *const ExportTableViewControllerSelectedItemKey;

@interface ExportTableViewController : UITableViewController

@end
