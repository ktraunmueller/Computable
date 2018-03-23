//
//  NotebookBrowserCell.h
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const NotebookBrowserCellDidRequestDeleteNotification;

@interface NotebookBrowserCell : UICollectionViewCell

@property (nonatomic, copy) NSString *notebookFile;
@property (nonatomic, readwrite) IBOutlet UIImageView *thumbnailView;
@property (nonatomic, readwrite) IBOutlet UITextField *titleField;
@property (nonatomic, readwrite) IBOutlet UIView *deleteTarget;
@property (nonatomic, readwrite) IBOutlet UIImageView *lockIcon;
@property (nonatomic, readwrite) IBOutlet UIImageView *deleteButton;
@property (nonatomic, readwrite) IBOutlet UIView *deleteButtonBackgroundView;

- (void)startWiggling;
- (void)stopWiggling;
- (void)ignoreWiggling:(BOOL)ignore;

@end
