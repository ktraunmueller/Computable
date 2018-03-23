//
//  FontSetting.h
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FontSetting : NSObject

@property (nonatomic, copy) NSString *displayName;
@property (nonatomic, copy) NSString *familyName;
@property (nonatomic, copy) NSString *regularFontName;
@property (nonatomic, copy) NSString *italicFontName;
@property (nonatomic, copy) NSString *boldFontName;
@property (nonatomic, copy) NSString *boldItalicFontName;
@property (nonatomic, copy) NSString *headerFontName;
@property (nonatomic, copy) NSString *monospaceFamilyName;
@property (nonatomic, strong) NSArray *sizes;   // array of CGFloats
@property (nonatomic, assign) CGFloat size;

- (UIFont *)regularFont;
- (UIFont *)boldFont;
- (UIFont *)italicFont;
- (UIFont *)boldItalicFont;

@end
