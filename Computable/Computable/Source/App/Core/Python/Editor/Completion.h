//
//  Completion.h
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, CompletionType)
{
    CompletionTypeModule,
    CompletionTypeClass,
    CompletionTypeInstance,
    CompletionTypeFunction,
};

@interface Completion : NSObject

@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *type;
@property (nonatomic, strong) NSString *completion;
@property (nonatomic, strong) NSString *docString;

@end
