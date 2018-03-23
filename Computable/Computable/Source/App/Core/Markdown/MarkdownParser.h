//
//  MarkdownParser.h
//  Computable
//
//  Created by Karl Traunmüller on 22.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MarkdownParser : NSObject

@property (nonatomic, readonly) NSArray *tokens;
@property (nonatomic, readonly) BOOL hasMath;

- (void)parse:(NSString *)markdown;

@end
