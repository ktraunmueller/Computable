//
//  OutputCell.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Cell.h"

@class InputCell;

@interface OutputCell : Cell

@property (nonatomic, copy) NSString *textData;
@property (nonatomic, strong) UIImage *imageData;
@property (nonatomic, readonly, weak) InputCell *inputCell;
@property (nonatomic, strong) Message *response;
@property (nonatomic, assign) BOOL flash;
@property (nonatomic, readonly) BOOL isEmpty;

+ (OutputCell *)newInstanceForInputCell:(InputCell *)inputCell;

@end
