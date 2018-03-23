//
//  MarkdownParser.m
//  Computable
//
//  Created by Karl Traunmüller on 22.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "MarkdownParser.h"
#import "MarkdownToken.h"
#import "../../../Common/ThirdParty/libsoldout/markdown.h"

static NSString *const TeXRegexPattern = @"(\\$\\$?)([\\s\\S]+?)\\1";
static NSString *const LaTeXRegexPattern = @"(\\\\begin\\{(.*?)\\})([\\s\\S]*?)\\\\end\\{\\2\\}";

static NSRegularExpression *TeXRegex;
static NSRegularExpression *LaTeXRegex;

@implementation MarkdownParser
{
    struct buf *_input;
    struct buf *_output;
    struct mkd_renderer _renderer;
    
    NSMutableArray *_tokens;
    NSMutableArray *_inlineTokens;
    
    MarkdownToken *_header;
    MarkdownToken *_paragraph;
    MarkdownToken *_blockQuote;
    MarkdownToken *_blockCode;
    MarkdownToken *_list;
    MarkdownToken *_listItem;
}

#pragma mark - Statics

+ (void)initialize
{
    NSError *error = nil;
    TeXRegex = [NSRegularExpression regularExpressionWithPattern:TeXRegexPattern options:0 error:&error];
    LaTeXRegex = [NSRegularExpression regularExpressionWithPattern:LaTeXRegexPattern options:0 error:&error];
}

#pragma mark - Properties

- (NSArray *)tokens
{
    return _tokens;
}

#pragma mark - MarkdownParser

- (id)init
{
    if (self = [super init])
    {
        mkd_enable_tables = 0;  // so that we can write $P(X|A)$ without escaping the pipe
        
        _input = bufnew(1024);
        _output = bufnew(1024);
        _renderer = md_renderer;
        _renderer.opaque = (__bridge void *)(self);
    }
    return self;
}

- (void)dealloc
{
    bufrelease(_input);
    bufrelease(_output);
}

- (void)parse:(NSString *)md
{
    _tokens = [NSMutableArray array];
    _inlineTokens = [NSMutableArray array];
    
    if (md == nil)
    {
        return;
    }
    _hasMath = NO;
    md = [self escapeMath:md];
    
    bufreset(_input);
    bufgrow(_input, md.length + 1);
    bufputs(_input, [md UTF8String]);
    bufreset(_output);
    markdown(_output, _input, &_renderer);
}

- (NSString *)safeString:(struct buf *)buf
{
    if (buf == NULL || buf->size == 0)
    {
        return nil;
    }
    return [[NSString alloc] initWithBytes:buf->data length:buf->size encoding:NSUTF8StringEncoding];
}

- (void)appendHeader:(NSString *)header level:(NSUInteger)level
{
    _header = [MarkdownToken tokenWithKind:MarkdownTokenKindHeader text:header];
    _header.level = level;
    [_tokens addObject:_header];
    [self setInlineTokens:_header];
}

- (void)appendParagraph:(NSString *)paragraph
{
    _paragraph = [MarkdownToken tokenWithKind:MarkdownTokenKindParagraph text:paragraph];
    [_tokens addObject:_paragraph];
    [self setInlineTokens:_paragraph];
}

- (void)appendBlockQuote:(NSString *)quote
{
    _blockQuote = [MarkdownToken tokenWithKind:MarkdownTokenKindBlockQuote text:quote];
    [_tokens addObject:_blockQuote];
    
    if (quote == nil)
    {
        // delete last paragraph, replace with block-quoted version
        if (_paragraph)
        {
            _blockQuote.text = _paragraph.text;
            _inlineTokens = _paragraph.inlineTokens;
            [_tokens removeObject:_paragraph];
            _paragraph = nil;
        }
    }
    [self setInlineTokens:_blockQuote];
}

- (void)appendBlockCode:(NSString *)code
{
    _blockCode = [MarkdownToken tokenWithKind:MarkdownTokenKindBlockCode text:code];
    [_tokens addObject:_blockCode];
    
    if (code == nil)
    {
        // delete last paragraph, replace with block-code version
        if (_paragraph)
        {
            _blockCode.text = _paragraph.text;
            _inlineTokens = _paragraph.inlineTokens;
            [_tokens removeObject:_paragraph];
            _paragraph = nil;
        }
    }
    [self setInlineTokens:_blockCode];
}

- (void)appendList:(NSString *)string
{
    _list = nil;
}

- (void)appendListItem:(NSString *)item ordered:(BOOL)ordered
{
    if (item == nil)
    {
        // delete last paragraph, replace with list item
        if (_paragraph)
        {
            item = _paragraph.text;
            _inlineTokens = _paragraph.inlineTokens;
            [_tokens removeObject:_paragraph];
            _paragraph = nil;
        }
    }
    _listItem = [MarkdownToken tokenWithKind:MarkdownTokenKindListItem text:item];
    _listItem.ordered = ordered;
    [self setInlineTokens:_listItem];
    
    if (_list == nil)
    {
        _list = [MarkdownToken tokenWithKind:MarkdownTokenKindList text:nil];
        [_tokens addObject:_list];
        _list.items = [NSMutableArray array];
    }
    [_list.items addObject:_listItem];
}

- (void)setInlineTokens:(MarkdownToken *)token
{
    if ([_inlineTokens count] > 0)
    {
        token.inlineTokens = _inlineTokens;
    }
    _inlineTokens = [NSMutableArray array];
}

- (void)inlineToken:(MarkdownToken *)token
{
    [_inlineTokens addObject:token];
}

- (NSMutableArray *)inlineTokens
{
    return _inlineTokens;
}

- (void)removeInlineTokens
{
    [_inlineTokens removeAllObjects];
}

- (NSString *)escapeMath:(NSString *)text
{
    if (text == nil)
    {
        return nil;
    }
    NSMutableString *work = [text mutableCopy];
    
    NSTextCheckingResult *match = [LaTeXRegex firstMatchInString:work options:0 range:NSMakeRange(0, work.length)];
    while (match)
    {
        NSRange delimiterNameRange = [match rangeAtIndex:2];
        NSString *delimiter = [work substringWithRange:delimiterNameRange];
        NSUInteger startDelimiterLength = [NSString stringWithFormat:@"\\begin{%@}", delimiter].length;
        NSUInteger endDelimiterLength = [NSString stringWithFormat:@"\\end{%@}", delimiter].length;
        NSRange matchRange = [match rangeAtIndex:3];
        if (matchRange.location == NSNotFound)
        {
            break;
        }
        _hasMath = YES;
        NSRange mathRange = NSMakeRange(matchRange.location - startDelimiterLength, matchRange.length + startDelimiterLength + endDelimiterLength);
        [work insertString:@"`" atIndex:mathRange.location + mathRange.length];
        [work insertString:@"`" atIndex:mathRange.location];
        NSUInteger location = matchRange.location + matchRange.length + endDelimiterLength + 2;
        match = [LaTeXRegex firstMatchInString:work options:0 range:NSMakeRange(location, work.length - location)];
    }
    
    match = [TeXRegex firstMatchInString:work options:0 range:NSMakeRange(0, work.length)];
    while (match)
    {
        NSRange delimiterRange = [match rangeAtIndex:1];
        if (delimiterRange.location > 0 && [[work substringWithRange:NSMakeRange(delimiterRange.location - 1, 1)] isEqualToString:@"\\"])
        {
            // single, escaped dollar sign (e.g., \$250)
            NSUInteger location = delimiterRange.location + 2;
            match = [TeXRegex firstMatchInString:work options:0 range:NSMakeRange(location, work.length - location)];
            continue;
        }
        NSRange matchRange = [match rangeAtIndex:2];
        if (matchRange.location == NSNotFound)
        {
            break;
        }
        // check for escaped LaTeX within matched range (see e.g., CFD_07_Step_5.ipynb)
        NSString *trimmedOfBackticksInRange = [work stringByReplacingOccurrencesOfString:@"`" withString:@"" options:0 range:matchRange];
        if (trimmedOfBackticksInRange.length < work.length)
        {
            work = [NSMutableString stringWithString:trimmedOfBackticksInRange];
            NSUInteger location = delimiterRange.location;
            // repeat this substitution
            match = [TeXRegex firstMatchInString:work options:0 range:NSMakeRange(location, work.length - location)];
            continue;
        }
        _hasMath = YES;
        NSRange mathRange = NSMakeRange(matchRange.location - delimiterRange.length, matchRange.length + 2 * delimiterRange.length);
        [work insertString:@"`" atIndex:mathRange.location + mathRange.length];
        [work insertString:@"`" atIndex:mathRange.location];
        NSUInteger location = matchRange.location + matchRange.length + delimiterRange.length + 2;
        match = [TeXRegex firstMatchInString:work options:0 range:NSMakeRange(location, work.length - location)];
    }
    return work;
}

- (NSUInteger)utf8strlen:(char *)str bytes:(int)bytes
{
    char *utf8 = malloc(bytes + 1);
    bzero(utf8, bytes + 1);
    memcpy(utf8, str, bytes);
    NSString *string = [NSString stringWithUTF8String:utf8];
    free(utf8);
    return string.length;
}

#pragma mark - Parser Callbacks

static void md_prolog(struct buf *ob, void *opaque)
{
}

static void md_epilog(struct buf *ob, void *opaque)
{
}

/* block level callbacks - NULL skips the block */
static void md_blockcode(struct buf *ob, struct buf *text, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    [parser appendBlockCode:string];
    bufreset(ob);
}

static void md_blockquote(struct buf *ob, struct buf *text, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    [parser appendBlockQuote:string];
    bufreset(ob);
}

//static void md_blockhtml(struct buf *ob, struct buf *text, void *opaque)
//{
//
//}

static void md_header(struct buf *ob, struct buf *text, int level, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    [parser appendHeader:string level:level];
    bufreset(ob);
}

static void md_paragraph(struct buf *ob, struct buf *text, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    [parser appendParagraph:string];
    bufreset(ob);
}

static void md_hrule(struct buf *ob, void *opaque)
{
    
}

static void md_list(struct buf *ob, struct buf *text, int flags, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    [parser appendList:string];
    bufreset(ob);
}

static void md_listitem(struct buf *ob, struct buf *text, int flags, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    BOOL ordered = flags & MKD_LIST_ORDERED;
    [parser appendListItem:string ordered:ordered];
    bufreset(ob);
}

static void md_table(struct buf *ob, struct buf *head_row, struct buf *rows, void *opaque)
{
    
}

static void md_table_cell(struct buf *ob, struct buf *text, int flags, void *opaque)
{
    
}

static void md_table_row(struct buf *ob, struct buf *cells, int flags, void *opaque)
{
    
}

/* span level callbacks - NULL or return 0 prints the span verbatim */
static int md_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque)
{
    //    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    //    Fragment *fragment = [Fragment new];
    //    fragment.linkTarget = [parser safeString:link];
    //    fragment.string = [parser link:fragment.linkContent target:fragment.linkTarget title:fragment.linkTitle];
    //    fragment.position = ob->size;
    //    [parser appendInlineFragment:fragment];
    //    return 1;
    return 0;
}

static int md_codespan(struct buf *ob, struct buf *text, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    MarkdownToken *token = [MarkdownToken tokenWithKind:MarkdownTokenKindCodeSpan text:string];
    token.position = [parser utf8strlen:ob->data bytes:ob->size]; // ob->size;
    [parser inlineToken:token];
    return 1;
}

static int md_emphasis(struct buf *ob, struct buf *text, char c, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    MarkdownToken *token = [MarkdownToken tokenWithKind:MarkdownTokenKindEmphasis text:string];
    token.position = [parser utf8strlen:ob->data bytes:ob->size]; // ob->size;
    [parser inlineToken:token];
    return 1;
}

static int md_double_emphasis(struct buf *ob, struct buf *text, char c, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    MarkdownToken *token = [MarkdownToken tokenWithKind:MarkdownTokenKindDoubleEmphasis text:string];
    token.position = [parser utf8strlen:ob->data bytes:ob->size]; // ob->size;
    [parser inlineToken:token];
    return 1;
}

static int md_triple_emphasis(struct buf *ob, struct buf *text, char c, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    NSString *string = [parser safeString:text];
    MarkdownToken *token = [MarkdownToken tokenWithKind:MarkdownTokenKindTripleEmphasis text:string];
    token.position = [parser utf8strlen:ob->data bytes:ob->size]; // ob->size;
    [parser inlineToken:token];
    return 1;
}

static int md_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque)
{
    return 0;
}

static int md_linebreak(struct buf *ob, void *opaque)
{
    // TODO
    //    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    //    [parser appendInline:[parser text:@"\n"] atPosition:ob->size];
    return 1;
}

static int md_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque)
{
    MarkdownParser *parser = (__bridge MarkdownParser *)(opaque);
    
    NSString *linkTarget = [parser safeString:link];
    NSString *linkContent = [parser safeString:content];
    NSString *linkTitle = [parser safeString:title];
    
    MarkdownToken *token = [MarkdownToken tokenWithKind:MarkdownTokenKindLink text:linkContent ? linkContent : linkTarget];
    token.position = [parser utf8strlen:ob->data bytes:ob->size]; // ob->size;
    token.linkContent = linkContent;
    token.linkTarget = linkTarget;
    token.linkTitle = linkTitle;
    NSMutableArray *inlineTokens = [parser inlineTokens];
    if (linkContent == nil && [inlineTokens count] > 0)
    {
        // transfer inline tokens to link
        if (token.position == 0 || [inlineTokens count] == 1)
        {
            // if link token position is zero or only one token, eat all inline tokens
            token.inlineTokens = [inlineTokens mutableCopy];
            [parser removeInlineTokens];
        }
        else
        {
            if ([inlineTokens count] > 1)
            {
                // eat all inline tokens starting from the first inline token with a position less than the previous one
                for (NSUInteger i = 1; i < [inlineTokens count]; i++)
                {
                    MarkdownToken *previousToken = [inlineTokens objectAtIndex:i - 1];
                    MarkdownToken *currentToken = [inlineTokens objectAtIndex:i];
                    if (currentToken.position < previousToken.position)
                    {
                        token.inlineTokens = [NSMutableArray array];
                        for (NSUInteger j = i; j < [inlineTokens count]; j++)
                        {
                            [token.inlineTokens addObject:[inlineTokens objectAtIndex:j]];
                        }
                        [inlineTokens removeObjectsInRange:NSMakeRange(i, [inlineTokens count] - i)];
                        break;
                    }
                }
            }
        }
    }
    [parser inlineToken:token];
    
    return 1;
}

//static int md_raw_html_tag(struct buf *ob, struct buf *tag, void *opaque)
//{
//    return 0;
//}

/* low level callbacks - NULL copies input directly into the output */
void md_entity(struct buf *ob, struct buf *entity, void *opaque)
{
    
}

void md_normal_text(struct buf *ob, struct buf *text, void *opaque)
{
}

struct mkd_renderer md_renderer =
{
	/* document-level callbacks */
	md_prolog,
	md_epilog,
    
	/* block-level callbacks */
	md_blockcode,
	md_blockquote,
	md_blockcode,
	md_header,
	md_hrule,
	md_list,
	md_listitem,
	md_paragraph,
	md_table,
	md_table_cell,
	md_table_row,
    
	/* span-level callbacks */
	md_autolink,
	md_codespan,
	md_double_emphasis,
	md_emphasis,
	md_image,
	md_linebreak,
	md_link,
	md_codespan,
	md_triple_emphasis,
    
	/* low-level callbacks */
	NULL,//md_entity,
	NULL,//md_normal_text,
    
	/* renderer data */
	64,
	"*_",
	NULL
};

@end
