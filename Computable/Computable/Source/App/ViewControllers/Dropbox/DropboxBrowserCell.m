//
//  DropboxBrowserCell.m
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 23.05.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import "DropboxBrowserCell.h"
#import "../../Core/Dropbox/Dropbox.h"

#import <Dropbox/Dropbox.h>

NSString *const DropboxBrowserCellIdentifier = @"DropboxBrowserCell";

@implementation DropboxBrowserCell

#pragma mark - DropboxBrowserCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:reuseIdentifier])
    {
    }
    return self;
}

@end
