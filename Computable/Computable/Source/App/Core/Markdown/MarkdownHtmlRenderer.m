//
//  MarkdownHtmlRenderer.m
//  Computable
//
//  Created by Karl Traunmüller on 13.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "MarkdownHtmlRenderer.h"
#import "MarkdownToken.h"
#import "../Settings/Settings.h"

NSString *const MathJaxReadyCallback = @"callback:mathjax-ready";

static NSString *const HtmlTemplate =
@"<!DOCTYPE html>"
"<head>"
"<meta charset='utf-8'></meta>"
"<style>%@</style>"
"%@%@"
"<body>%@</body>";

static NSString *const HeaderTemplate = @"<h%d>%@</h%d>";
static NSString *const ParagraphTemplate = @"<p>%@</p>";
static NSString *const BlockQuoteTemplate = @"<div class='blockquote' lang='en'><q>%@</q></div>";
static NSString *const BlockCodeTemplate = @"<pre class='blockcode'>%@</pre>";
static NSString *const ListItemTemplate = @"<li>%@</li>";
static NSString *const OrderedListTemplate = @"<ol>%@</ol>";
static NSString *const UnorderedListTemplate = @"<ul>%@</ul>";
static NSString *const BoldTemplate = @"<span class='bold'>%@</span>";
static NSString *const ItalicTemplate = @"<span class='italic'>%@</span>";
static NSString *const BoldItalicTemplate = @"<span class='bolditalic'>%@</span>";
static NSString *const MonospaceTemplate = @"<span class='monospace'>%@</span>";
static NSString *const LinkTemplate = @"<a href='%@' target='_blank'>%@</a>";

static NSString *const MathJaxSupport =
@"<script type='text/x-mathjax-config'>"
"MathJax.Hub.Config({"
"   imageFont: null,"
"   MathMenu: { showRenderer: false },"
"   messageStyle: 'none',"
"   styles: {'#MathJax_Message': {display: 'none !important'}},"
"   extensions: ['tex2jax.js','TeX/AMSmath.js','TeX/AMSsymbols.js'],"
"   jax: ['input/TeX','output/HTML-CSS'],"
"   tex2jax: {"
"   inlineMath: [ ['$','$'] ],"
"   displayMath: [ ['$$','$$'] ],"
"   processEscapes: false }"
"});"
"</script>"
"<script type='text/javascript' src='mathjax/MathJax.js?config=TeX-AMS_HTML'></script>"
"<script>"
"MathJax.Hub.Register.StartupHook('End',function () {"
"   window.location.href = 'callback:mathjax-ready';"
"});"
"</script>";

static NSString *const ClientRectsOfLinks =
@"<script type='text/javascript'>"
"function clientRectsOfLinks() {"
"   var result = '';"
"	var anchors = document.getElementsByTagName('a');"
"	for (var i = 0; i < anchors.length; i++) {"
"       var a = anchors[i];"
"		var encoded = a.href + '|';"
"		var rects = a.getClientRects();"
"		for (var j = 0; j < rects.length; j++) {"
"			encoded += rects[j].left + ',' + rects[j].top + ',' + rects[j].width + ',' + rects[j].height + ';';"
"	    }"
"	    encoded += '\\n';"
"	    result += encoded;"
"	}"
"   return result;"
"}"
"</script>";

@implementation MarkdownHtmlRenderer
{
    NSMutableString *_body;
    BOOL _needsMathJax;
    NSString *_styles;
}

#pragma mark - Statics

+ (NSURL *)baseUrl
{
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"base" withExtension:@"html" subdirectory:@"html"];
    return [url URLByDeletingLastPathComponent];
}

#pragma mark - MarkdownRenderer

- (id)init
{
    if (self = [super init])
    {
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

- (NSString *)render:(NSArray *)tokens withMath:(BOOL)withMath
{
    _body = [NSMutableString new];
    _needsMathJax = withMath;
    for (MarkdownToken *token in tokens)
    {
        [self renderToken:token];
    }
    [self trimWhitespace];
    
    NSString *html = [NSString stringWithFormat:HtmlTemplate, [self styles], _needsMathJax ? MathJaxSupport : @"", ClientRectsOfLinks, _body];
//    NSLog(@">> HTML ------------------------------------------------");
//    NSLog(@"%@", html);
//    NSLog(@"<< HTML ------------------------------------------------");
    return html;
}

- (void)renderToken:(MarkdownToken *)token
{
    _needsMathJax |= token.hasMath;

    switch (token.kind)
    {
        case MarkdownTokenKindHeader:
            [self renderHeader:token];
            break;
        case MarkdownTokenKindParagraph:
            [self renderParagraph:token];
            break;
        case MarkdownTokenKindBlockCode:
            [self renderBlockCode:token];
            break;
        case MarkdownTokenKindBlockQuote:
            [self renderBlockQuote:token];
            break;
        case MarkdownTokenKindList:
            [self renderList:token];
            break;
//        case MarkdownTokenKindHRule:
//            [self renderHRule:token];
//            break;
//        case MarkdownTokenKindLineBreak:
//            [self renderLineBreak:token];
//            break;
        default:
            break;
    }
}

- (void)renderHeader:(MarkdownToken *)token
{
    NSString *innerHtml = [self renderContent:token];
    NSString *header = [self wrapHeader:innerHtml level:token.level];
    [_body appendString:header];
}

- (void)renderParagraph:(MarkdownToken *)token
{
    NSString *innerHtml = [self renderContent:token];
    NSString *paragraph = [self wrapParagraph:innerHtml];
    [_body appendString:paragraph];
}

- (void)renderBlockQuote:(MarkdownToken *)token
{
    NSString *innerHtml = [self renderContent:token];
    NSString *blockQuote = [self wrapBlockquote:innerHtml];
    [_body appendString:blockQuote];
}

- (void)renderBlockCode:(MarkdownToken *)token
{
    NSString *innerHtml = [self renderContent:token];
    NSString *blockCode = [self wrapBlockcode:innerHtml];
    [_body appendString:blockCode];
}

- (void)renderList:(MarkdownToken *)token
{
    if ([token.items count] == 0)
    {
        return;
    }
    NSMutableArray *listItems = [NSMutableArray array];
    for (MarkdownToken *item in token.items)
    {
        NSString *innerHtml = [self renderContent:item];
        NSString *listItem = [self wrapListItem:innerHtml ordered:token.ordered];
        [listItems addObject:listItem];
    }
    NSString *innerHtml = [listItems componentsJoinedByString:@"\n"];
    NSString *list = [self wrapList:innerHtml ordered:token.ordered];
    [_body appendString:list];
}

- (NSString *)renderContent:(MarkdownToken *)token
{
    NSMutableString *innerHtml = [[NSMutableString alloc] initWithString:token.text ? token.text : @""];
    [self renderInlineTokensOf:token into:innerHtml];
    [innerHtml replaceOccurrencesOfString:@"\n" withString:@"<br/>" options:0 range:NSMakeRange(0, innerHtml.length)];
    return innerHtml;
}

- (void)renderInlineTokensOf:(MarkdownToken *)token into:(NSMutableString *)string
{
    if ([token.inlineTokens count] == 0)
    {
        return;
    }
    
    [token.inlineTokens sortUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        MarkdownToken *t1 = obj1;
        MarkdownToken *t2 = obj2;
        return t1.position < t2.position ? NSOrderedAscending : (t1.position > t2.position ? NSOrderedDescending : NSOrderedSame);
    }];

    NSUInteger lastTokenPosition = 0;
    NSUInteger position = 0;
    for (MarkdownToken *inlineToken in token.inlineTokens)
    {
        NSString *fragment = [self renderInline:inlineToken];
        if (string.length == 0 && position == 0)
        {
            position = [fragment rangeOfString:inlineToken.text].location;
        }
        else
        {
            position += inlineToken.position - lastTokenPosition;
        }
        if (position < string.length)
        {
            [string insertString:fragment atIndex:position];
            position += fragment.length;
        }
        else
        {
            [string appendString:fragment];
        }
        lastTokenPosition = inlineToken.position;
    }
}

- (NSString *)renderInline:(MarkdownToken *)token
{
    switch (token.kind)
    {
        case MarkdownTokenKindCodeSpan: return [self monospace:token.text];
        case MarkdownTokenKindEmphasis: return [self textItalic:token.text];
        case MarkdownTokenKindDoubleEmphasis: return [self textBold:token.text];
        case MarkdownTokenKindTripleEmphasis: return [self textBoldItalic:token.text];
        case MarkdownTokenKindLink: return [self link:token];
            //        case MarkdownTokenKindImage: return nil;
        default:
            break;
    }
    return @"";
}

- (NSString *)wrapHeader:(NSString *)header level:(NSUInteger)level
{
    if (header == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:HeaderTemplate, level, header, level];
}

- (NSString *)wrapParagraph:(NSString *)paragraph
{
    if (paragraph == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:ParagraphTemplate, paragraph];
}

- (NSString *)wrapBlockquote:(NSString *)quote
{
    if (quote == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:BlockQuoteTemplate, quote];
}

- (NSString *)wrapBlockcode:(NSString *)code
{
    if (code == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:BlockCodeTemplate, code];
}

- (NSString *)wrapList:(NSString *)items ordered:(BOOL)ordered
{
    if (items == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:ordered ? OrderedListTemplate : UnorderedListTemplate, items];
}

- (NSString *)wrapListItem:(NSString *)item ordered:(BOOL)ordered
{
    if (item == nil)
    {
        return @"";
    }
    item = [item stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]; // make canonical (remove leading/trailing newlines)
    return [NSString stringWithFormat:ListItemTemplate, item];
}

- (NSString *)text:(NSString *)string
{
    if (string == nil)
    {
        return @"";
    }
    return string;
}

- (NSString *)textBold:(NSString *)string
{
    if (string == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:BoldTemplate, string];
}

- (NSString *)textItalic:(NSString *)string
{
    if (string == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:ItalicTemplate, string];
}

- (NSString *)textBoldItalic:(NSString *)string
{
    if (string == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:BoldItalicTemplate, string];
}

- (NSString *)monospace:(NSString *)string
{
    if (string == nil)
    {
        return @"";
    }
    return [NSString stringWithFormat:MonospaceTemplate, string];
}

- (NSString *)link:(MarkdownToken *)token
{
    if (token.linkContent == nil && token.linkTarget == nil && [token.inlineTokens count] == 0)
    {
        return @"";
    }

    NSMutableString *innerHtml = [[NSMutableString alloc] initWithString:token.linkContent ? token.linkContent : @""];
    [self renderInlineTokensOf:token into:innerHtml];
    return [NSString stringWithFormat:LinkTemplate, token.linkTarget, innerHtml.length > 0 ? innerHtml : token.linkTarget];
}

#pragma mark - Notifications

- (void)fontSettingsDidChange:(NSNotification *)notification
{
    _styles = nil;
}

#pragma mark - Utility Functions

- (void)trimWhitespace
{
    if (_body == nil)
    {
        return;
    }
    NSString *trimmed = [_body stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if (trimmed.length != _body.length)
    {
        _body = [[NSMutableString alloc] initWithString:trimmed];
    }
}

- (NSString *)styles
{
    if (_styles == nil)
    {
        NSURL *templateUrl = [[NSBundle mainBundle] URLForResource:@"template" withExtension:@"css" subdirectory:@"html/css"];
        NSError *error = nil;
        NSString *template = [NSString stringWithContentsOfURL:templateUrl encoding:NSUTF8StringEncoding error:&error];
        if (error)
        {
            NSLog(@"error reading stylesheet template: %@", error);
        }
        Settings *settings = [Settings instance];
        _styles = [template stringByReplacingOccurrencesOfString:@"[TextFontFamily]" withString:settings.textFont.familyName];
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[HeaderFontFamily]" withString:settings.textFont.headerFontName];
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[BoldFontFamily]" withString:settings.textFont.boldFontName];
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[ItalicFontFamily]" withString:settings.textFont.italicFontName];
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[BoldItalicFontFamily]" withString:settings.textFont.boldItalicFontName];
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[BaseFontSize]" withString:[NSString stringWithFormat:@"%.0f", settings.textFont.size]];
        
        NSString *monospaceFontFamily = settings.textFont.monospaceFamilyName ? settings.textFont.monospaceFamilyName : settings.codeFont.familyName;
        _styles = [_styles stringByReplacingOccurrencesOfString:@"[MonospaceFontFamily]" withString:monospaceFontFamily];
    }
    return _styles;
}

@end
