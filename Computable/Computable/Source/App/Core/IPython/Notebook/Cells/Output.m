//
//  Output.m
//  Computable
//
//  Created by Karl Traunmüller on 21.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Output.h"

NSString *const kOutputTypeStream = @"stream";
NSString *const kOutputStreamStdout = @"stdout";
NSString *const kOutputStreamStderr = @"stderr";
NSString *const kOutputTypeDisplayData = @"display_data";
NSString *const kOutputTypePyout = @"pyout";
NSString *const kOutputTypePyerr = @"pyerr";

NSString *const kJsonPromptNumber = @"prompt_number";
NSString *const kJsonOutputText = @"text";
NSString *const kJsonOutputPng = @"png";

@implementation Output

#pragma mark - Statics

+ (OutputType)outputTypeFromString:(NSString *)outputType stream:(NSString *)stream
{
    if ([outputType isEqualToString:kOutputTypeStream])
    {
        if ([stream isEqualToString:kOutputStreamStdout])
        {
            return OutputTypeStdout;
        }
        if ([stream isEqualToString:kOutputStreamStderr])
        {
            return OutputTypeStderr;
        }
    }
    if ([outputType isEqualToString:kOutputTypePyout])
    {
        return OutputTypePyout;
    }
    if ([outputType isEqualToString:kOutputTypePyerr])
    {
        return OutputTypePyerr;
    }
    if ([outputType isEqualToString:kOutputTypeDisplayData])
    {
        return OutputTypeDisplayData;
    }
    NSLog(@"unkown output type %@", outputType);
    return OutputTypeUnknown;
}

+ (Output *)newStreamOutputWithType:(NSString *)stream text:(NSString *)text
{
    Output *output = [Output new];
    output.type = [Output outputTypeFromString:kOutputTypeStream stream:stream];
    output.stream = stream;
    output.text = text;
    return output;
}

+ (Output *)newPyOutOutputWithText:(NSString *)text
{
    Output *output = [Output new];
    output.type = OutputTypePyout;
    output.text = text;
    return output;
}

+ (Output *)newPyErrOutputWithEname:(NSString *)ename evalue:(NSString *)evalue traceback:(NSArray *)traceback
{
    Output *output = [Output new];
    output.type = OutputTypePyerr;
    output.ename = ename;
    output.evalue = evalue;
    output.traceback = traceback;
    return output;
}

+ (Output *)newDisplayDataOutput:(NSString *)base64Png
{
    Output *output = [Output new];
    output.type = OutputTypeDisplayData;
    output.base64Png = base64Png;
    return output;
}

@end
