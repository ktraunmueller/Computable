//
//  SimpleTextStorage.m
//  Computable
//
//  Created by Karl Traunmüller on 02.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "SimpleTextStorage.h"

@implementation SimpleTextStorage
{
    NSMutableAttributedString *_attributedString;
}

#pragma mark - SimpleTextStorage

- (id)init
{
    if (self = [super init])
    {
        _attributedString = [NSMutableAttributedString new];
    }
    return self;
}

- (void)updateWithString:(NSAttributedString *)string
{
    [_attributedString replaceCharactersInRange:NSMakeRange(0, _attributedString.length) withAttributedString:string];
}

#pragma mark - NSTextStorage

- (NSString *)string
{
    return _attributedString.string;
}

- (NSDictionary *)attributesAtIndex:(NSUInteger)location effectiveRange:(NSRangePointer)range
{
    return [_attributedString attributesAtIndex:location effectiveRange:range];
}

- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)str
{
    [self beginEditing];
    [_attributedString replaceCharactersInRange:range withString:str];
    [self edited:NSTextStorageEditedCharacters
           range:range
  changeInLength:(NSInteger)str.length - (NSInteger)range.length];
    [self endEditing];
}

- (void)setAttributes:(NSDictionary *)attrs range:(NSRange)range
{
    [self beginEditing];
    [_attributedString setAttributes:attrs range:range];
    [self edited:NSTextStorageEditedAttributes range:range changeInLength:0];
    [self endEditing];
}

@end
