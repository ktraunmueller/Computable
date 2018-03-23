//
//  CellView.m
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookCellView.h"
#import "../../Core/IPython/Notebook/Cells/InputCell.h"
#import "../../Core/IPython/Notebook/Cells/OutputCell.h"

NSString *const NotebookCellHeightDidChangeNotification = @"NotebookCellHeightDidChangeNotification";

@implementation NotebookCellView

#pragma mark - Properties

- (InputCell *)inputCell
{
    if ([self.cell isKindOfClass:[InputCell class]])
    {
        return (InputCell *)self.cell;
    }
    if ([self.cell isKindOfClass:[OutputCell class]])
    {
        return ((OutputCell *)self.cell).inputCell;
    }
    return nil;
}

- (OutputCell *)outputCell
{
    if ([self.cell isKindOfClass:[InputCell class]])
    {
        return ((InputCell *)self.cell).outputCell;
    }
    if ([self.cell isKindOfClass:[OutputCell class]])
    {
        return (OutputCell *)self.cell;
    }
    return nil;
}

@end
