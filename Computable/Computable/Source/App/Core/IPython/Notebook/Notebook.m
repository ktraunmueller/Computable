//
//  Notebook.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Notebook.h"
#import "Cells/Cell.h"
#import "Cells/InputCell.h"
#import "Cells/OutputCell.h"
#import "Cells/Output.h"
#import "../Kernel/Kernel.h"
#import "../Session/Session.h"
#import "../../IPython/IPythonApplication.h"
#import "../../Python/Runtime/PythonRuntime.h"
#import "../../Python/Runtime/NSString+PythonRuntime.h"
#import "../../../../Common/Utilities/NSDictionary+JSON.h"
#import "../../../../Common/Utilities/UIImage+Base64.h"
#import "../../../../Common/Utilities/UIImage+Resizing.h"

static NSString *const kJsonFormatMajor = @"nbformat";
static NSString *const kJsonFormatMinor = @"nbformat_minor";
static NSString *const kJsonMetadata = @"metadata";
static NSString *const kJsonWorksheets = @"worksheets";
static NSString *const kJsonCells = @"cells";
static NSString *const kJsonCellType = @"cell_type";
static NSString *const kJsonCellHeadingLevel = @"level";
static NSString *const kJsonCellCollapsed = @"collapsed";
static NSString *const kJsonCellInput = @"input";
static NSString *const kJsonCellSource = @"source";
static NSString *const kJsonCellLanguage = @"language";
static NSString *const kJsonCellMetadata = @"metadata";
static NSString *const kJsonCellOutputs = @"outputs";
static NSString *const kJsonCellOutputText = @"text";
static NSString *const kJsonCellOutputPng = @"png";
static NSString *const kJsonCellOutputType = @"output_type";
static NSString *const kJsonCellOutputStream = @"stream";

static NSString *const kCellTypeCode = @"code";
static NSString *const kCellTypeMarkdown = @"markdown";
static NSString *const kCellTypeHeading = @"heading";
static NSString *const kCellTypeRaw = @"raw";

static const CFTimeInterval kMinSaveInterval = 5;

@implementation Notebook
{
    NSString *_path;
    NSString *_notebookId;
    Kernel *_kernel;
    NSMutableArray *_cells;
    CFAbsoluteTime _lastSave;
}

#pragma mark - Statics

+ (Notebook *)newInstanceWithKernel:(Kernel *)kernel path:(NSString *)path notebookId:(NSString *)notebookId
{
    return [[Notebook alloc] initWithKernel:kernel path:path notebookId:notebookId];
}

+ (NSString *)parseStrings:(NSDictionary *)json forKey:(NSString *)key
{
    id source = [json objectForKey:key];
    if ([source isKindOfClass:[NSArray class]])
    {
        source = [source componentsJoinedByString:@""];
    }
    return source;
}

+ (NSArray *)splitIntoLines:(NSString *)source
{
    if (source.length == 0)
    {
        return @[ ];
    }
    NSArray *lines = [source componentsSeparatedByString:@"\n"];
    if ([lines count] <= 1)
    {
        return lines;
    }
    NSMutableArray *splitLines = [NSMutableArray array];
    for (NSString *line in lines)
    {
        if ([lines indexOfObject:line] == [lines count] - 1)
        {
            if (line.length > 0)
            {
                [splitLines addObject:line];
            }
            break;
        }
        [splitLines addObject:[line stringByAppendingString:@"\n"]];
    }
    return splitLines;
}

+ (InputCellType)cellTypeFromString:(NSString *)cellType
{
    if ([cellType isEqualToString:kCellTypeCode])
    {
        return InputCellTypeCode;
    }
    if ([cellType isEqualToString:kCellTypeMarkdown])
    {
        return InputCellTypeMarkdown;
    }
    if ([cellType isEqualToString:kCellTypeHeading])
    {
        return InputCellTypeHeading;
    }
    if ([cellType isEqualToString:kCellTypeRaw])
    {
        return InputCellTypeRaw;
    }
    NSLog(@"unknown input cell type %@", cellType);
    return InputCellTypeUnknown;
}

+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key
{
    if ([key isEqualToString:@"focusedInputCell"])
    {
        return NO;
    }
    return [super automaticallyNotifiesObserversForKey:key];
}

#pragma mark - Properties

- (void)setFocusedInputCell:(InputCell *)focusedInputCell
{
    if (_focusedInputCell == focusedInputCell)
    {
        return;
    }
    BOOL wasEditing = _focusedInputCell.editing;
    if (_focusedInputCell.editing)
    {
        _focusedInputCell.preventResignFirstResponderOnEndEditing = YES;    // prevent keyboard from hiding
        _focusedInputCell.editing = NO;
    }
    if (_focusedInputCell.focused)
    {
        _focusedInputCell.focused = NO;
    }
    
    [self willChangeValueForKey:@"focusedInputCell"];
    _focusedInputCell = focusedInputCell;
    _focusedInputCellIndex = _focusedInputCell ? [_cells indexOfObject:_focusedInputCell] : NSNotFound;
    _focusedInputCell.focused = YES;
    [self didChangeValueForKey:@"focusedInputCell"];
    if (wasEditing)
    {
        _focusedInputCell.editing = YES;    // continue editing
    }
}

#pragma mark - Notebook

- (id)initWithKernel:(Kernel *)kernel path:(NSString *)path notebookId:(NSString *)notebookId
{
    if (self = [super init])
    {
        _formatMajor = 3;
        _formatMinor = 0;
        _path = path;
        _notebookId = notebookId;
        _kernel = kernel;
        _cells = [NSMutableArray array];
        _focusedInputCellIndex = NSNotFound;
    }
    return self;
}

#pragma mark - Execution

- (NSString *)expandPaths:(NSString *)source
{
    NSString *homeDirectory = NSHomeDirectory();
    source = [source stringByReplacingOccurrencesOfString:@"~/" withString:[homeDirectory stringByAppendingString:@"/"]];
    return source;
}

- (void)execute:(InputCell *)inputCell completion:(RequestBlock)completion
{
    inputCell.executing = YES;
    NSString *sourceWithExpandedPaths = [self expandPaths:inputCell.source];
    [_kernel execute:sourceWithExpandedPaths
          completion:^(Message *request, Message *response) {
              if (![response.parentMsgId isEqualToString:request.msgId])
              {
                  // response does not belong this request, ignore (for now)
                  return;
              }
              if (response.type == MessageTypeStatus)
              {
                  return;   // ignore (for now)
              }
              if (response.type == MessageTypePyIn)
              {
                  return;   // ignore input mirroring
              }
              if (response.type == MessageTypePyOut)
              {
                  [self updateOutput:response forInputCell:inputCell];
              }
              else if (response.type == MessageTypeStream)
              {
                  inputCell.flashStream = YES;
              }
              else if (response.type == MessageTypeDisplayData)
              {
                  [self updateOutput:response forInputCell:inputCell];
              }
              else if (response.type == MessageTypeExecuteReply)
              {
                  inputCell.executing = NO;
              }
              else if (response.type == MessageTypePyErr)
              {
                  inputCell.executing = NO;
              }
              if (completion)
              {
                  completion(request, response);
              }
          }];
}

- (void)updateOutput:(Message *)response forInputCell:(InputCell *)inputCell
{
    if (inputCell.outputCell == nil)
    {
        OutputCell *outputCell = [OutputCell newInstanceForInputCell:inputCell];
        outputCell.response = response;
        inputCell.outputCell = outputCell;
        NSUInteger inputCellIndex = [_cells indexOfObject:inputCell];
        if (inputCellIndex == [_cells count] - 1)
        {
            [_cells addObject:outputCell];
        }
        else
        {
            [_cells insertObject:outputCell atIndex:inputCellIndex + 1];
        }
    }
    else
    {
        inputCell.outputCell.response = response;
    }
    inputCell.outputCell.flash = YES;
}

- (void)interruptKernel
{
    for (Cell *cell in _cells)
    {
        if ([cell isKindOfClass:[InputCell class]])
        {
            InputCell *inputCell = (InputCell *)cell;
            if (inputCell.executing)
            {
                inputCell.executing = NO;
            }
        }
    }
//    [_kernel interrupt];  // TODO causes visit_decref crash 
}

#pragma mark - Cells

- (InputCell *)newInputCell
{
    if (_focusedInputCell.editing)
    {
        _focusedInputCell.preventResignFirstResponderOnEndEditing = YES;    // prevent keyboard from hiding
        _focusedInputCell.editing = NO;
    }
    if (_focusedInputCell.focused)
    {
        _focusedInputCell.focused = NO;
    }
//    [self willChangeValueForKey:@"focusedInputCell"];
    _focusedInputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeCode];
    [_cells addObject:_focusedInputCell];
    _focusedInputCellIndex = [_cells count] - 1;
    _focusedInputCell.focused = YES;
//    [self didChangeValueForKey:@"focusedInputCell"];
    return _focusedInputCell;
}

- (InputCell *)newInputCellBelowFocusedCell
{
    if (_focusedInputCellIndex == NSNotFound)
    {
        return [self newInputCell];
    }
    if (_focusedInputCell.editing)
    {
        _focusedInputCell.editing = NO;
    }
    if (_focusedInputCell.focused)
    {
        _focusedInputCell.focused = NO;
    }
    NSUInteger index = _focusedInputCellIndex;
    if (_focusedInputCell.outputCell && !_focusedInputCell.outputCell.isEmpty)
    {
        index = [_cells indexOfObject:_focusedInputCell.outputCell];
    }
//    [self willChangeValueForKey:@"focusedInputCell"];
    _focusedInputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeCode];
    if (index < [_cells count] - 1)
    {
        [_cells insertObject:_focusedInputCell atIndex:index + 1];
    }
    else
    {
        [_cells addObject:_focusedInputCell];
    }
    _focusedInputCell.focused = YES;
    _focusedInputCellIndex = [_cells indexOfObject:_focusedInputCell];
//    [self didChangeValueForKey:@"focusedInputCell"];
    return _focusedInputCell;
}

- (void)deleteFocusedInputCell
{
    if (_focusedInputCell == nil)
    {
        return;
    }
    [self willChangeValueForKey:@"focusedInputCell"];
    [_cells removeObject:_focusedInputCell];
    if (_focusedInputCell.outputCell)
    {
        [_cells removeObject:_focusedInputCell.outputCell];
    }
    _focusedInputCellIndex = NSNotFound;
    _focusedInputCell = nil;
    [self didChangeValueForKey:@"focusedInputCell"];
}

- (void)deleteOutputCellForFocusedInputCell
{
    if (_focusedInputCell.outputCell == nil)
    {
        return;
    }
    [_cells removeObject:_focusedInputCell.outputCell];
    _focusedInputCell.outputCell = nil;
}

- (NSUInteger)moveFocusedCellUp
{
    if (_focusedInputCellIndex == NSNotFound)
    {
        return NSNotFound;
    }
    if (_focusedInputCellIndex == 0)
    {
        return _focusedInputCellIndex;
    }
    NSRange range = NSMakeRange(_focusedInputCellIndex, _focusedInputCell.outputCell ? 2 : 1);
    [_cells removeObjectsInRange:range];
    if ([_cells count] == 0)
    {
        return NSNotFound;
    }

    Cell *previousCell = [_cells objectAtIndex:--_focusedInputCellIndex];
    if (_focusedInputCellIndex > 0 && [previousCell isKindOfClass:[OutputCell class]])
    {
        --_focusedInputCellIndex;
    }
    [_cells insertObject:_focusedInputCell atIndex:_focusedInputCellIndex];
    if (_focusedInputCell.outputCell)
    {
        [_cells insertObject:_focusedInputCell.outputCell atIndex:_focusedInputCellIndex + 1];
    }
    return _focusedInputCellIndex;
}

- (NSUInteger)moveFocusedCellDown
{
    if (_focusedInputCellIndex == NSNotFound)
    {
        return NSNotFound;
    }
    if (_focusedInputCellIndex == [_cells count] - 1)
    {
        return _focusedInputCellIndex;
    }
    NSRange range = NSMakeRange(_focusedInputCellIndex, _focusedInputCell.outputCell ? 2 : 1);
    [_cells removeObjectsInRange:range];
    if ([_cells count] == 0)
    {
        return NSNotFound;
    }

    Cell *nextCell = [_cells objectAtIndex:_focusedInputCellIndex];
    _focusedInputCellIndex += ((InputCell *)nextCell).outputCell ? 2 : 1;
    if (_focusedInputCellIndex <= [_cells count] - 1)
    {
        [_cells insertObject:_focusedInputCell atIndex:_focusedInputCellIndex];
        if (_focusedInputCell.outputCell)
        {
            [_cells insertObject:_focusedInputCell.outputCell atIndex:_focusedInputCellIndex + 1];
        }
    }
    else
    {
        [_cells addObject:_focusedInputCell];
        if (_focusedInputCell.outputCell)
        {
            [_cells addObject:_focusedInputCell.outputCell];
        }
    }
    return _focusedInputCellIndex;
}

- (BOOL)isFirstInputCell:(InputCell *)inputCell
{
    return [_cells firstObject] == inputCell;
}

- (BOOL)isLastInputCell:(InputCell *)inputCell
{
    for (int i = [_cells count] - 1; i >= 0; i--)
    {
        Cell *cell = [_cells objectAtIndex:i];
        if ([cell isKindOfClass:[inputCell class]])
        {
            return cell == inputCell;
        }
    }
    return NO;
}

- (BOOL)isLastMarkdownInputCell:(InputCell *)inputCell
{
    for (int i = [_cells count] - 1; i >= 0; i--)
    {
        Cell *cell = [_cells objectAtIndex:i];
        if ([cell isKindOfClass:[inputCell class]])
        {
            InputCell *iCell = (InputCell *)cell;
            if (iCell.isMarkdown)
            {
                return iCell == inputCell;
            }
        }
    }
    return NO;
}

- (InputCell *)nextCodeInputCell:(InputCell *)inputCell
{
    if (inputCell == nil)
    {
        return nil;
    }
    NSUInteger index = [_cells indexOfObject:inputCell];
    if (index == NSNotFound || index == [_cells count] - 1)
    {
        return nil;
    }
    for (NSUInteger i = index + 1; i < [_cells count]; i++)
    {
        Cell *cell = [_cells objectAtIndex:i];
        if ([cell isKindOfClass:[InputCell class]] && ((InputCell *)cell).type == InputCellTypeCode)
        {
            return (InputCell *)cell;
        }
    }
    return nil;
}

- (NSString *)combinedSourceUpToAndIncluding:(InputCell *)inputCell
{
    if (inputCell == nil)
    {
        return nil;
    }
    NSMutableArray *sources = [NSMutableArray array];
    for (Cell *cell in _cells)
    {
        if ([cell isKindOfClass:[InputCell class]])
        {
            InputCell *anInputCell = (InputCell *)cell;
            if (anInputCell.source != nil)
            {
                [sources addObject:anInputCell.source];
            }
            if (anInputCell == inputCell)
            {
                break;
            }
        }
    }
    return [sources componentsJoinedByString:@"\n"];
}

- (void)invalidateCells
{
    for (Cell *cell in _cells)
    {
        [cell invalidate];
    }
}

#pragma mark - Load

- (void)loadWithOutputCells:(BOOL)createOutputCells
{
    NSData *data = [NSData dataWithContentsOfFile:_path];
    if (data == nil)
    {
        return;
    }
    NSError *error = nil;
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
    if (error)
    {
        // TODO indicate error to user
        NSLog(@"%@", error);
        return;
    }
    _formatMajor = [[json objectForKey:kJsonFormatMajor] intValue];
    _formatMinor = [[json objectForKey:kJsonFormatMinor] intValue];
    _format = [NSString stringWithFormat:@"%d.%d", (int)_formatMajor, (int)_formatMinor];
    _metadata = [json objectForKey:kJsonMetadata];
    NSArray *worksheets = [json objectForKey:kJsonWorksheets];
    NSDictionary *worksheet = [worksheets count] > 0 ? [worksheets objectAtIndex:0] : nil;
    [self parseWorksheet:worksheet createOutputCells:createOutputCells];
}

- (void)parseWorksheet:(NSDictionary *)worksheetJson createOutputCells:(BOOL)createOutputCells
{
    _cells = [NSMutableArray array];
    NSArray *cellsJson = [worksheetJson objectForKey:kJsonCells];
    for (NSDictionary *cellJson in cellsJson)
    {
        NSString *cellTypeString = [cellJson objectForKey:kJsonCellType];
        InputCellType type = [Notebook cellTypeFromString:cellTypeString];
        switch (type)
        {
            case InputCellTypeCode:
                [self parseCodeCell:cellJson createOutputCells:createOutputCells];
                break;
            case InputCellTypeMarkdown:
                [self parseMarkdownCell:cellJson];
                break;
            case InputCellTypeHeading:
                [self parseHeadingCell:cellJson];
                break;
            case InputCellTypeRaw:
                [self parseRawCell:cellJson];
                break;
            default:
                break;
        }
    }
}

- (InputCell *)parseCodeCell:(NSDictionary *)cellJson createOutputCells:(BOOL)createOutputCells
{
    if (cellJson == nil)
    {
        return nil;
    }
    
    InputCell *inputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeCode];
    [_cells addObject:inputCell];
//    inputCell.type = InputCellTypeCode;
    inputCell.collapsed = [[cellJson objectForKey:kJsonCellCollapsed] boolValue];
    inputCell.source = [Notebook parseStrings:cellJson forKey:kJsonCellInput];
    inputCell.language = [cellJson objectForKey:kJsonCellLanguage];
    inputCell.metadata = [cellJson objectForKey:kJsonCellMetadata];
    inputCell.executionCount = [[cellJson objectForKey:kJsonPromptNumber] integerValue];
    
    NSArray *outputsJson = [cellJson objectForKey:kJsonCellOutputs];
    if ([outputsJson count] == 0)
    {
        return inputCell;
    }
    OutputCell *outputCell = nil;
    if (createOutputCells)
    {
        outputCell = [OutputCell newInstanceForInputCell:inputCell];
        [_cells addObject:outputCell];
    }
    
    NSMutableArray *streamOutLines = [NSMutableArray array];
    for (NSDictionary *outputJson in outputsJson)
    {
        if (inputCell.outputs == nil)
        {
            inputCell.outputs = [NSMutableArray array];
        }
        OutputType outputType = [Output outputTypeFromString:[outputJson objectForKey:kJsonCellOutputType]
                                                      stream:[outputJson objectForKey:kJsonCellOutputStream]];
        switch (outputType)
        {
            case OutputTypeStdout:
                [streamOutLines addObject:[Notebook parseStrings:outputJson forKey:kJsonOutputText]];
                break;
            case OutputTypeStderr:
                // ignore?
                break;
            case OutputTypePyout:
                [self parsePyout:outputJson intoOutputCell:outputCell inputCell:inputCell];
                break;
            case OutputTypePyerr:
                break;
            case OutputTypeDisplayData:
                [self parseDisplayData:outputJson intoOutputCell:outputCell inputCell:inputCell];
                break;
            default:
                break;
        }
    }
    // TODO this is not entirely correct: if stream output is mixed with other output, the order of outputs is changed
    if ([streamOutLines count] > 0)
    {
        NSString *streamOut = [streamOutLines componentsJoinedByString:@"\n"];
        streamOut = [streamOut stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        streamOut = [streamOut stringByStrippingPythonPath];
        inputCell.streamOut = streamOut;
        for (NSString *line in streamOutLines)
        {
            Output *output = [Output newStreamOutputWithType:kOutputStreamStdout text:line];
            [inputCell.outputs addObject:output];
        }
    }
    if (outputCell && outputCell.textData == nil && outputCell.imageData == nil)
    {
        [_cells removeObject:outputCell];
    }
    return inputCell;
}

- (void)parsePyout:(NSDictionary *)json intoOutputCell:(OutputCell *)outputCell inputCell:(InputCell *)inputCell
{
    if (json == nil)
    {
        return;
    }
    
    NSString *textData = [Notebook parseStrings:json forKey:kJsonOutputText];
    if (outputCell)
    {
        outputCell.textData = textData;
        outputCell.executionCount = [[json objectForKey:kJsonPromptNumber] integerValue];
        if (outputCell.executionCount > 0)
        {
            outputCell.inputCell.executionCount = outputCell.executionCount;
        }
    }
    Output *output = [Output newPyOutOutputWithText:textData];
    [inputCell.outputs addObject:output];
}

- (void)parseDisplayData:(NSDictionary *)json intoOutputCell:(OutputCell *)outputCell inputCell:(InputCell *)inputCell
{
    if (json == nil)
    {
        return;
    }
    NSString *textData = [Notebook parseStrings:json forKey:kJsonOutputText];;
    if (outputCell)
    {
        outputCell.executionCount = [[json objectForKey:kJsonPromptNumber] integerValue];
        outputCell.imageData = [UIImage imageFromBase64String:[json objectForKey:kJsonOutputPng] useDeviceScale:NO];
        outputCell.cachedHeight = outputCell.imageData.size.height;
        outputCell.textData = textData;
        if (outputCell.executionCount > 0)
        {
            outputCell.inputCell.executionCount = outputCell.executionCount;
        }
    }
    Output *output = [Output newDisplayDataOutput:[json objectForKey:kJsonOutputPng]];
    output.text = outputCell.textData;
    [inputCell.outputs addObject:output];
}

- (InputCell *)parseMarkdownCell:(NSDictionary *)cellJson
{
    if (cellJson == nil)
    {
        return nil;
    }
    
    InputCell *inputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeMarkdown];
    [_cells addObject:inputCell];
//    inputCell.type = InputCellTypeMarkdown;
    inputCell.source = [Notebook parseStrings:cellJson forKey:kJsonCellSource];
    return inputCell;
}

- (InputCell *)parseHeadingCell:(NSDictionary *)cellJson
{
    if (cellJson == nil)
    {
        return nil;
    }
    
    InputCell *inputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeHeading];
    [_cells addObject:inputCell];
//    inputCell.type = InputCellTypeHeading;
    inputCell.headingLevel = [[cellJson objectForKey:kJsonCellHeadingLevel] unsignedIntegerValue];
    inputCell.source = [Notebook parseStrings:cellJson forKey:kJsonCellSource];
    return inputCell;
}

- (InputCell *)parseRawCell:(NSDictionary *)cellJson
{
    if (cellJson == nil)
    {
        return nil;
    }
    
    InputCell *inputCell = [[InputCell alloc] initWithNotebook:self type:InputCellTypeRaw];
    [_cells addObject:inputCell];
//    inputCell.type = InputCellTypeRaw;
    inputCell.source = [Notebook parseStrings:cellJson forKey:kJsonCellSource];
    return inputCell;
}

#pragma mark - Save

- (void)save:(BOOL)force
{
    if (!force && CFAbsoluteTimeGetCurrent() - _lastSave < kMinSaveInterval)
    {
        return;
    }
    _lastSave = CFAbsoluteTimeGetCurrent();
    
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:_metadata ? _metadata : @{ } forKey:kJsonMetadata];
    [json setObject:@(_formatMajor) forKey:kJsonFormatMajor];
    [json setObject:@(_formatMinor) forKey:kJsonFormatMinor];
    
    NSMutableArray *worksheets = [NSMutableArray array];
    [json setObject:worksheets forKey:kJsonWorksheets];
    
    NSMutableDictionary *worksheet = [NSMutableDictionary dictionary];
    [worksheet setObject:@{ } forKey:kJsonMetadata];
    [worksheets addObject:worksheet];
    
    NSMutableArray *cellsJson = [NSMutableArray array];
    [worksheet setObject:cellsJson forKey:kJsonCells];
    for (Cell *cell in _cells)
    {
        if (![cell isKindOfClass:[InputCell class]])
        {
            continue;
        }
        InputCell *inputCell = (InputCell *)cell;
        NSDictionary *cellJson = [self cellJson:inputCell];
        if (cellJson)
        {
            [cellsJson addObject:cellJson];
        }
        [inputCell resetDirty];
    }
    [json writeToFile:_path];
}

- (NSDictionary *)cellJson:(InputCell *)cell
{
    if (cell == nil)
    {
        return nil;
    }
    switch (cell.type)
    {
        case InputCellTypeCode: return [self codeCellJson:cell];
        case InputCellTypeMarkdown: return [self markdownCellJson:cell];
        case InputCellTypeHeading: return [self headingCellJson:cell];
        case InputCellTypeRaw: return [self rawCellJson:cell];
        default:
            break;
    }
    return nil;
}

- (NSDictionary *)codeCellJson:(InputCell *)cell
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kCellTypeCode forKey:kJsonCellType];
    
    [json setObject:[Notebook splitIntoLines:cell.source] forKey:kJsonCellInput];
    [json setObject:cell.language ? cell.language : @"python" forKey:kJsonCellLanguage];
    [json setObject:cell.metadata ? cell.metadata : @{ } forKey:kJsonCellMetadata];
    [json setObject:cell.collapsed ? @"true" : @"false" forKey:kJsonCellCollapsed];
    [json setObject:@(cell.executionCount) forKey:kJsonPromptNumber];
    
    NSMutableArray *outputs = [NSMutableArray array];
    [json setObject:outputs forKey:kJsonCellOutputs];
    for (Output *output in cell.outputs)
    {
        NSDictionary *outputJson = nil;
        switch (output.type)
        {
            case OutputTypeStdout:
                outputJson = [self output:output toStreamJsonOfType:kOutputStreamStdout];
                break;
            case OutputTypeStderr:
                outputJson = [self output:output toStreamJsonOfType:kOutputStreamStderr];
                break;
            case OutputTypePyout:
                outputJson = [self outputToPyout:output promptNumber:cell.executionCount];
                break;
            case OutputTypePyerr:
                break;
            case OutputTypeDisplayData:
                outputJson = [self outputToDisplayData:output];
                break;
            default:
                break;
        }
        if (outputJson)
        {
            [outputs addObject:outputJson];
        }
    }
    return json;
}

- (NSDictionary *)output:(Output *)output toStreamJsonOfType:(NSString *)streamType
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kOutputTypeStream forKey:kJsonCellOutputType];
    [json setObject:streamType forKey:kJsonCellOutputStream];
    [json setObject:[Notebook splitIntoLines:output.text] forKey:kJsonCellOutputText];
    return json;
}

- (NSDictionary *)outputToPyout:(Output *)output promptNumber:(NSUInteger)promptNumber
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kOutputTypePyout forKey:kJsonCellOutputType];
    [json setObject:@(promptNumber) forKey:kJsonPromptNumber];
    [json setObject:[Notebook splitIntoLines:output.text] forKey:kJsonCellOutputText];
    return json;
}

- (NSDictionary *)outputToDisplayData:(Output *)output
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kOutputTypeDisplayData forKey:kJsonCellOutputType];
    [json setObject:[Notebook splitIntoLines:output.text] forKey:kJsonCellOutputText];
    if (output.base64Png)
    {
        NSString *base64Png = output.base64Png;
        if ([UIScreen mainScreen].scale == 2)
        {
            // scale down
            UIImage *image = [UIImage imageFromBase64String:base64Png useDeviceScale:YES];
            if (image)
            {
                image = [image resizedImageWithWidth:image.size.width / 2];
                base64Png = [image base64String];
            }
        }
        if (base64Png)
        {
            [json setObject:base64Png forKey:kJsonCellOutputPng];
        }
    }
    return json;
}

- (NSDictionary *)markdownCellJson:(InputCell *)cell
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kCellTypeMarkdown forKey:kJsonCellType];
    [json setObject:cell.metadata ? cell.metadata : @{ } forKey:kJsonCellMetadata];
    [json setObject:[Notebook splitIntoLines:cell.source] forKey:kJsonCellSource];
    return json;
}

- (NSDictionary *)headingCellJson:(InputCell *)cell
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kCellTypeHeading forKey:kJsonCellType];
    [json setObject:@(cell.headingLevel) forKey:kJsonCellHeadingLevel];
    [json setObject:cell.metadata ? cell.metadata : @{ } forKey:kJsonCellMetadata];
    [json setObject:[Notebook splitIntoLines:cell.source] forKey:kJsonCellSource];
    return json;
}

- (NSDictionary *)rawCellJson:(InputCell *)cell
{
    NSMutableDictionary *json = [NSMutableDictionary dictionary];
    [json setObject:kCellTypeRaw forKey:kJsonCellType];
    [json setObject:cell.metadata ? cell.metadata : @{ } forKey:kJsonCellMetadata];
    [json setObject:[Notebook splitIntoLines:cell.source] forKey:kJsonCellSource];
    return json;
}

@end
