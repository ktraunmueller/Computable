//
//  MarkdownToken.h
//  Computable
//
//  Created by Karl Traunmüller on 01.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, MarkdownTokenKind)
{
    MarkdownTokenKindUnknown,
    
    MarkdownTokenKindHeader,
    MarkdownTokenKindParagraph,
    
    MarkdownTokenKindBlockCode,
    MarkdownTokenKindBlockQuote,
    
    MarkdownTokenKindList,
    MarkdownTokenKindListItem,
    
    MarkdownTokenKindHRule,
    MarkdownTokenKindLineBreak,
    
    MarkdownTokenKindCodeSpan,
    MarkdownTokenKindEmphasis,
    MarkdownTokenKindDoubleEmphasis,
    MarkdownTokenKindTripleEmphasis,
    MarkdownTokenKindLink,
    MarkdownTokenKindImage,
    
    MarkdownTokenKindMath,
};

@class MarkdownSourceLine;

@interface MarkdownToken : NSObject

@property (nonatomic, assign) MarkdownTokenKind kind; // assign to make it overridable (e.g., headings)
@property (nonatomic, copy) NSString *text;
@property (nonatomic, assign) NSUInteger line;      // source line, set by renderer
@property (nonatomic, assign) NSUInteger column;    // source column, set by renderer
@property (nonatomic, assign) NSUInteger position;  // position within parent string, for rendering inline tokens
@property (nonatomic, assign) NSUInteger level;     // headers
@property (nonatomic, assign) BOOL ordered;         // lists
@property (nonatomic, assign) BOOL hasMath;         // paragraph with TeX
@property (nonatomic, strong) NSString *linkTarget; // links
@property (nonatomic, strong) NSString *linkContent;
@property (nonatomic, strong) NSString *linkTitle;
@property (nonatomic, strong) NSMutableArray *inlineTokens;
@property (nonatomic, strong) NSMutableArray *items;

+ (MarkdownToken *)tokenWithKind:(MarkdownTokenKind)kind text:(NSString *)text;

@end
