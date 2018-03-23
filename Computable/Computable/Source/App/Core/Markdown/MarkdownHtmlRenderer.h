//
//  MarkdownHtmlRenderer.h
//  Computable
//
//  Created by Karl Traunmüller on 13.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const MathJaxReadyCallback;

@interface MarkdownHtmlRenderer : NSObject

+ (NSURL *)baseUrl;

- (NSString *)render:(NSArray *)tokens withMath:(BOOL)withMath;

@end
