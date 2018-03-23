//
//  DropboxBrowserCell.h
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 23.05.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const DropboxBrowserCellIdentifier;

@class DBFileInfo;

@interface DropboxBrowserCell : UITableViewCell

@property (nonatomic, weak) DBFileInfo *fileInfo;

@end
