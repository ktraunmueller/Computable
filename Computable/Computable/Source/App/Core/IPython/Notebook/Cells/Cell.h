//
//  Cell.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Notebook;
@class Message;

@interface Cell : NSObject

@property (nonatomic, readonly, weak) Notebook *notebook;
@property (nonatomic, assign) BOOL collapsed;
@property (nonatomic, assign) NSUInteger executionCount;
// layout caching
@property (nonatomic, assign) CGFloat cachedHeight;
// reference to UI
@property (nonatomic, weak) id context;

- (id)initWithNotebook:(Notebook *)notebook;

- (void)invalidate;

@end
