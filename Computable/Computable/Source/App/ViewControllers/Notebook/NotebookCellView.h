//
//  NotebookCellView.h
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const NotebookCellHeightDidChangeNotification;

@class Cell;
@class InputCell;
@class OutputCell;
@class NotebookViewController;

@interface NotebookCellView : UICollectionViewCell

@property (nonatomic, weak) Cell *cell;
@property (nonatomic, readonly) InputCell *inputCell;
@property (nonatomic, readonly) OutputCell *outputCell;
@property (nonatomic, weak) NotebookViewController *notebookController;

@end
