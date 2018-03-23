//
//  OutputCell.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "OutputCell.h"
#import "InputCell.h"
#import "Output.h"
#import "../Notebook.h"
#import "../../Session/Message.h"
#import "../../../Python/Runtime/NSString+PythonRuntime.h"

@interface OutputCell ()

@property (nonatomic, weak) InputCell *inputCell;

@end

@implementation OutputCell

#pragma mark - Statics

+ (OutputCell *)newInstanceForInputCell:(InputCell *)inputCell
{
    OutputCell *cell = [[OutputCell alloc] initWithNotebook:inputCell.notebook];
    cell.inputCell = inputCell;
    inputCell.outputCell = cell;
    return cell;
}

#pragma mark - Properties

- (void)setInputCell:(InputCell *)inputCell
{
    _inputCell = inputCell;
    self.executionCount = self.inputCell.executionCount;
}

- (void)setResponse:(Message *)response
{
    _response = response;

    if (_response.executionCount > 0)
    {
        self.executionCount = _response.executionCount;
    }
    _textData = [[_response outputForMimeType:MimeTypeTextPlain] stringByStrippingPythonPath];
    _imageData = [_response outputForMimeType:MimeTypeImagePng];
    self.cachedHeight = _imageData.size.height;
}

- (BOOL)isEmpty
{
    return _textData.length == 0 && _imageData == nil;
}

- (NSString *)debugDescription
{
    return [NSString stringWithFormat:@"%@: %@", [self class], [self.textData substringToIndex:MIN(20, self.textData.length)]];
}

@end
