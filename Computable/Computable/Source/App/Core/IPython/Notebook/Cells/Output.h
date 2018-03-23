//
//  Output.h
//  Computable
//
//  Created by Karl Traunmüller on 21.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const kOutputTypeStream;
extern NSString *const kOutputStreamStdout;
extern NSString *const kOutputStreamStderr;
extern NSString *const kOutputTypeDisplayData;
extern NSString *const kOutputTypePyout;
extern NSString *const kOutputTypePyerr;

extern NSString *const kJsonPromptNumber;
extern NSString *const kJsonOutputText;
extern NSString *const kJsonOutputPng;

typedef NS_ENUM(NSUInteger, OutputType)
{
    OutputTypeUnknown,
    OutputTypeStdout,   // stream type (added to input cell)
    OutputTypeStderr,   // stream type (added to input cell)
    OutputTypePyout,
    OutputTypePyerr,
    OutputTypeDisplayData,
};

@interface Output : NSObject

@property (nonatomic, copy) NSDictionary *metadata;
@property (nonatomic, assign) OutputType type;
@property (nonatomic, copy) NSString *stream;
@property (nonatomic, copy) NSString *ename;
@property (nonatomic, copy) NSString *evalue;
@property (nonatomic, copy) NSArray *traceback;
@property (nonatomic, copy) NSString *text;
@property (nonatomic, strong) NSString *base64Png;

+ (OutputType)outputTypeFromString:type stream:(NSString *)stream;

+ (Output *)newStreamOutputWithType:(NSString *)stream text:(NSString *)text;
+ (Output *)newPyOutOutputWithText:(NSString *)text;
+ (Output *)newPyErrOutputWithEname:(NSString *)ename evalue:(NSString *)evalue traceback:(NSArray *)traceback;
+ (Output *)newDisplayDataOutput:(NSString *)base64Png;

@end
