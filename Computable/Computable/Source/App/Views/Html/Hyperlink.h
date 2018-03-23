//
//  Hyperlink.h
//  Computable
//
//  Created by Karl Traunmüller on 28.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Hyperlink : NSObject

@property (nonatomic, copy) NSString *url;
@property (nonatomic, strong) NSArray *clientRects;

@end
