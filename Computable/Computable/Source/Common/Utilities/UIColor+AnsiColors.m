//
//  UIColor+AnsiColors.m
//  Computable
//
//  Created by Karl Traunmüller on 08.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIColor+AnsiColors.h"

@implementation UIColor (AnsiColors)

+ (UIColor *)ansiColor:(SGRParameter)color
{
    if (color == SGRParameterResetNormal)
    {
        return [UIColor blackColor];
    }
    if (color >= SGRParameterSetTextColor && color < SGRParameterSetBackgroundColor)
    {
        int hue = color - SGRParameterSetTextColor;
        switch (hue) {
            case 0: return [UIColor blackColor];
            case 1: return [UIColor redColor];
            case 2: return [UIColor greenColor];
            case 3: return [UIColor yellowColor];
            case 4: return [UIColor blueColor];
            case 5: return [UIColor magentaColor];
            case 6: return [UIColor cyanColor];
            case 7: return [UIColor whiteColor];
            default:
                break;
        }
    }
    if (color >= SGRParameterSetBackgroundColor)
    {
        int hue = color - SGRParameterSetBackgroundColor;
        switch (hue) {
            case 0: return [UIColor blackColor];
            case 1: return [UIColor redColor];
            case 2: return [UIColor greenColor];
            case 3: return [UIColor yellowColor];
            case 4: return [UIColor blueColor];
            case 5: return [UIColor magentaColor];
            case 6: return [UIColor cyanColor];
            case 7: return [UIColor whiteColor];
            default:
                break;
        }
    }
    return nil;
}

@end
