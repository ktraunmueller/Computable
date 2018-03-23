//
//  InputCell.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "InputCell.h"
#import "OutputCell.h"
#import "Output.h"
#import "../Notebook.h"
#import "../../../Markdown/MarkdownParser.h"
#import "../../../Markdown/MarkdownToken.h"
#import "../../../Markdown/MarkdownHtmlRenderer.h"
#import "../../../Python/Runtime/NSString+PythonRuntime.h"

NSString *const InputCellDidChangeTypeNotification = @"InputCellDidChangeTypeNotification";

@implementation InputCell
{
    MarkdownParser *_markdownParser;
    MarkdownHtmlRenderer *_markdownRenderer;
    NSString *_markdownHtml;
}

#pragma mark - Statics

+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key
{
    if ([key isEqualToString:@"dirty"] ||
        [key isEqualToString:@"focused"] ||
        [key isEqualToString:@"editing"] ||
        [key isEqualToString:@"executing"] ||
        [key isEqualToString:@"type"])
    {
        return NO;
    }
    return [super automaticallyNotifiesObserversForKey:key];
}

#pragma mark - Properties

- (void)setFocused:(BOOL)focused
{
    if (_focused != focused)
    {
        [self willChangeValueForKey:@"focused"];
        _focused = focused;
        [self didChangeValueForKey:@"focused"];
    }
}

- (void)setEditing:(BOOL)editing
{
    if (_editing != editing)
    {
        [self willChangeValueForKey:@"editing"];
        _editing = editing;
        [self didChangeValueForKey:@"editing"];
    }
}

- (void)setExecuting:(BOOL)executing
{
    if (_executing != executing)
    {
        [self willChangeValueForKey:@"executing"];
        _executing = executing;
        [self didChangeValueForKey:@"executing"];
    }
}

- (void)setDirty:(BOOL)dirty
{
    if (_dirty != dirty)
    {
        [self willChangeValueForKey:@"dirty"];
        _dirty = dirty;
        [self didChangeValueForKey:@"dirty"];
    }
}

- (BOOL)isMarkdown
{
    return _type == InputCellTypeMarkdown || _type == InputCellTypeHeading || _type == InputCellTypeRaw;
}

- (void)setType:(InputCellType)type
{
    if (_type != type)
    {
        _streamOut = nil;
        _outputs = nil;
        _responses = nil;
        _executeReply = nil;
        _errorResponse = nil;
        _cachedSourceHeight = 0;
        _renderedMarkdownImage = nil;
        _hyperlinks = nil;

        [self willChangeValueForKey:@"type"];
        _type = type;
        [self didChangeValueForKey:@"type"];
    }
}

- (void)setSource:(NSString *)source
{
    if ([_source isEqualToString:source])
    {
        return;
    }
    _markdownHtml = nil;
    _cachedSourceHeight = 0;
    _renderedMarkdownImage = nil;
    [self willChangeValueForKey:@"dirty"];
    _dirty = _source != nil;
    _source = source;
    [self didChangeValueForKey:@"dirty"];
}

- (NSString *)markdownHtml
{
    if (self.isMarkdown)
    {
        if (_markdownParser == nil)
        {
            _markdownParser = [MarkdownParser new];
        }
        if (_markdownRenderer == nil)
        {
            _markdownRenderer = [MarkdownHtmlRenderer new];
        }
        if (_markdownHtml == nil)
        {
            if (_type == InputCellTypeHeading)
            {
                [_markdownParser parse:_source];
                if ([_markdownParser.tokens count] > 0)
                {
                    MarkdownToken *header = [_markdownParser.tokens firstObject];
                    header.kind = MarkdownTokenKindHeader;
                    header.level = _headingLevel;
                }
                _markdownHtml = [_markdownRenderer render:_markdownParser.tokens withMath:_markdownParser.hasMath];
            }
            else
            {
                [_markdownParser parse:_type == InputCellTypeRaw ? [NSString stringWithFormat:@"    %@", _source] : _source];
                _markdownHtml = [_markdownRenderer render:_markdownParser.tokens withMath:_markdownParser.hasMath];
            }
        }
    }
    return _markdownHtml;
}

- (BOOL)hasMath
{
    return _markdownParser.hasMath;
}

#pragma mark - InputCell

- (id)initWithNotebook:(Notebook *)notebook  type:(InputCellType)type
{
    if (self = [super initWithNotebook:notebook])
    {
        // possible fix for: https://rink.hockeyapp.net/manage/apps/61508/app_versions/42/crash_reasons/15991347
        _type = type;
        _selectedRange = NSMakeRange(0, 0);
    }
    return self;
}

- (void)execute:(dispatch_block_t)completion
{
    if (self.source.length == 0)
    {
        return;
    }
    if (_executing)
    {
        NSLog(@"ignoring execute request, cell already executing");
        return;
    }
    __block NSMutableArray *responses = [NSMutableArray array];
    _responses = responses;
    _streamOut = nil;
    _outputs = [NSMutableArray array];
    _executeReply = nil;
    _errorResponse = nil;
    [self.notebook execute:self
                completion:^(Message *request, Message *response) {
                    //                NSLog(@"%@", response);
                    [responses addObject:response];
                    if (response.isStatus)
                    {
                        return;
                    }
                    if (response.isStream)
                    {
                        Output *output = [Output newStreamOutputWithType:response.streamName text:response.streamData];
                        [_outputs addObject:output];
                        
                        _streamOut = _streamOut == nil ? response.streamData : [[_streamOut stringByAppendingString:@"\n"] stringByAppendingString:response.streamData];
                        _streamOut = [_streamOut stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
                        _streamOut = [_streamOut stringByStrippingPythonPath];
                    }
                    else if (response.isOutput)
                    {
                        Output *output = [Output newPyOutOutputWithText:[response outputForMimeType:MimeTypeTextPlain]];
                        [_outputs addObject:output];
                    }
                    else if (response.isDisplayData)
                    {
                        Output *output = [Output newDisplayDataOutput:[response rawContentForMimeType:MimeTypeImagePng]];
                        [_outputs addObject:output];
                    }
                    else if (response.isError)
                    {
                        Output *output = [Output newPyErrOutputWithEname:response.errorName evalue:response.errorValue traceback:response.errorTraceback];
                        [_outputs addObject:output];
                        
                        _errorResponse = response;
                    }
                    else if (response.isExecuteReply)
                    {
                        _executeReply = response;
                        self.executionCount = _executeReply.executionCount;
                        self.outputCell.executionCount = self.executionCount;
                    }
                    if (completion)
                    {
                        completion();
                    }
                }];
}

- (void)resetDirty
{
    _dirty = NO;
}

- (void)invalidate
{
    [super invalidate];
    
    _markdownHtml = nil;
    _renderedMarkdownImage = nil;
    _cachedSourceHeight = 0;
}

- (NSString *)debugDescription
{
    return [NSString stringWithFormat:@"%@: %@", [self class], [self.source substringToIndex:MIN(20, self.source.length)]];
}

@end
