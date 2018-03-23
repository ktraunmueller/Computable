//
//  PythonTextStorage.m
//  Computable
//
//  Created by Karl Traunmüller on 03.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonTextStorage.h"
#import "../../../Core/Settings/Settings.h"
#import "../../../../Common/Utilities/UIColor+Hex.h"

static UIColor *PythonKeywordColor;
static UIColor *PythonMagicColor;
static UIColor *PythonNumberColor;
static UIColor *PythonStringColor;
static UIColor *PythonCommentColor;

static NSString *const KeywordRegexPattern = @"(?<!\\w)(import|from|as|and|assert|break|class|continue|def|del|elif|else|except|exec|finally|for|global|if|in|is|lambda|not|or|pass|print|raise|return|try|while|yield|abs|apply|buffer|callable|chr|cmp|coerce|compile|complex|copyright|credits|delattr|dir|divmod|eval|execfile|exit|filter|float|getattr|globals|hasattr|hash|hex|id|input|int|intern|isinstance|issubclass|iter|len|license|list|locals|long|map|max|min|oct|open|ord|pow|quit|range|raw_input|reduce|reload|repr|round|setattr|slice|str|tuple|type|unichr|unicode|vars|xrange|zip|None|self)(?!\\w)";
static NSString *const MagicRegexPattern = @"(?<!\\w)(%%?\\w+)";
static NSString *const NumberRegexPattern = @"(?<!\\w)(((0x[0-9a-fA-F]+)|(([0-9]+\\.?[0-9]*|\\.[0-9]+)([eE][-+]?[0-9]+)?))[fFlLuU]{0,2})(?!\\w)";
static NSString *const StringRegexPattern = @"('(\\.|[^'\\n])*'|\"(\\.|[^\"\\n])*\")";
static NSString *const TripleQuotedStringRegexPattern = @"(\"\"\"(?:[^\"]|\\\"|\"{1,2}(?!\"))*\"\"\"|'''(?:[^']|\\'|'{1,2}(?!'))*''')";
//static NSString *const CommentRegexPattern = @"(#[^\"\\n\\r]*(?:\"[^\"\\n\\r]*\"[^\"\\n\\r]*)*[\\r\\n])";
static NSString *const CommentRegexPattern = @"(#[^\\n\\r]*)";

static NSRegularExpression *KeywordRegex;
static NSRegularExpression *MagicRegex;
static NSRegularExpression *NumberRegex;
static NSRegularExpression *StringRegex;
static NSRegularExpression *TripleQuotedStringRegex;
static NSRegularExpression *CommentRegex;
static NSArray *Regexes;

static NSDictionary *NormalAttributes;
static NSDictionary *KeywordAttributes;
static NSDictionary *MagicAttributes;
static NSDictionary *NumberAttributes;
static NSDictionary *StringAttributes;
static NSDictionary *CommentAttributes;
static NSArray *Attributes;

@implementation PythonTextStorage
{
    NSMutableAttributedString *_backingStore;
}

#pragma mark - Statics

+ (void)initialize
{
    PythonKeywordColor = [UIColor colorWithHex:0x88419d alpha:1.0];
//    PythonMagicColor = [UIColor colorWithHex:0x002b36 alpha:1.0];
    PythonMagicColor = [UIColor colorWithHex:0x586e75 alpha:1.0];
    PythonNumberColor = [UIColor colorWithHex:0x08519c alpha:1.0];
    PythonStringColor = [UIColor colorWithHex:0x006d2c alpha:1.0];
    PythonCommentColor = [UIColor colorWithWhite:0.4 alpha:1.0];
    
    NSError *error = nil;
    KeywordRegex = [NSRegularExpression regularExpressionWithPattern:KeywordRegexPattern options:0 error:&error];
    MagicRegex = [NSRegularExpression regularExpressionWithPattern:MagicRegexPattern options:0 error:&error];
    NumberRegex = [NSRegularExpression regularExpressionWithPattern:NumberRegexPattern options:0 error:&error];
    StringRegex = [NSRegularExpression regularExpressionWithPattern:StringRegexPattern options:0 error:&error];
    TripleQuotedStringRegex = [NSRegularExpression regularExpressionWithPattern:TripleQuotedStringRegexPattern options:0 error:&error];
    CommentRegex = [NSRegularExpression regularExpressionWithPattern:CommentRegexPattern options:0 error:&error];
    Regexes = @[ KeywordRegex, MagicRegex, NumberRegex, StringRegex, CommentRegex ];
    
    [PythonTextStorage updateFontStyles];
}

+ (void)updateFontStyles
{
    FontSetting *codeFont = [Settings instance].codeFont;
    KeywordAttributes = @{ NSFontAttributeName : codeFont.boldFont,
                           NSForegroundColorAttributeName : PythonKeywordColor };
    MagicAttributes = @{ NSFontAttributeName : codeFont.boldFont,
                         NSForegroundColorAttributeName : PythonMagicColor };
    NumberAttributes = @{ NSFontAttributeName : codeFont.regularFont,
                          NSForegroundColorAttributeName : PythonNumberColor };
    StringAttributes = @{ NSFontAttributeName :  codeFont.regularFont,
                          NSForegroundColorAttributeName : PythonStringColor };
    CommentAttributes = @{ NSFontAttributeName : codeFont.italicFont,
                           NSForegroundColorAttributeName : PythonCommentColor };
    NormalAttributes = @{ NSFontAttributeName : codeFont.regularFont,
                          NSForegroundColorAttributeName : [UIColor blackColor] };
    Attributes = @[ KeywordAttributes, MagicAttributes, NumberAttributes, StringAttributes, CommentAttributes ];
}

#pragma mark - PythonTextStorage

- (id)init
{
    if (self = [super init])
    {
        _backingStore = [NSMutableAttributedString new];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(fontSettingsDidChange:)
                                                     name:FontSettingsDidChangeNotification
                                                   object:nil];
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)applyStylesToRange:(NSRange)range
{
    [self setAttributes:NormalAttributes range:range];
    
    for (NSUInteger i = 0; i < [Regexes count]; i++)
    {
        NSRegularExpression *regex = Regexes[i];
        NSDictionary *attributes = Attributes[i];
        
        [regex enumerateMatchesInString:_backingStore.string
                                options:0
                                  range:range
                             usingBlock:^(NSTextCheckingResult *match, NSMatchingFlags flags, BOOL *stop) {
                                 NSRange matchRange = [match rangeAtIndex:1];
                                 [self setAttributes:attributes range:matchRange];
                             }];
    }
    
    [TripleQuotedStringRegex enumerateMatchesInString:_backingStore.string
                                              options:0
                                                range:NSMakeRange(0, _backingStore.string.length)
                                           usingBlock:^(NSTextCheckingResult *match, NSMatchingFlags flags, BOOL *stop) {
                                               NSRange matchRange = [match rangeAtIndex:1];
                                               [self setAttributes:StringAttributes range:matchRange];
                                           }];
}

#pragma mark - Notifications

- (void)fontSettingsDidChange:(NSNotification *)notification
{
    [PythonTextStorage updateFontStyles];
    
    NSRange range = NSMakeRange(0, _backingStore.string.length);
    [self applyStylesToRange:range];
}

#pragma mark - NSTextStorage

- (NSString *)string
{
    return _backingStore.string;
}

- (NSDictionary *)attributesAtIndex:(NSUInteger)location effectiveRange:(NSRangePointer)range
{
    return [_backingStore attributesAtIndex:location effectiveRange:range];
}

- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)str
{
    [self beginEditing];
    [_backingStore replaceCharactersInRange:range withString:str];
    [self edited:NSTextStorageEditedCharacters
           range:range
  changeInLength:(NSInteger)str.length - (NSInteger)range.length];
    [self endEditing];
}

- (void)setAttributes:(NSDictionary *)attrs range:(NSRange)range
{
    [self beginEditing];
    [_backingStore setAttributes:attrs range:range];
    [self edited:NSTextStorageEditedAttributes range:range changeInLength:0];
    [self endEditing];
}

- (void)processEditing
{
    NSRange extendedRange = NSUnionRange(self.editedRange, [_backingStore.string lineRangeForRange:NSMakeRange(self.editedRange.location, 0)]);
//    NSRange paragaphRange = [_backingStore.string paragraphRangeForRange:self.editedRange];
    [self applyStylesToRange:extendedRange];
    
    [super processEditing];
}

@end

