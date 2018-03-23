//
//  Settings.h
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "FontSetting.h"

extern NSString *const FontSettingsDidChangeNotification;

@interface Settings : NSObject

@property (nonatomic, readonly) FontSetting *classicTextFont;
@property (nonatomic, readonly) FontSetting *modernTextFont;
@property (nonatomic, readonly) NSArray *availableTextFonts;
@property (nonatomic, strong) FontSetting *textFont;

@property (nonatomic, readonly) NSArray *availableCodeFonts;
@property (nonatomic, strong) FontSetting *codeFont;

+ (Settings *)instance;

- (void)save;

@end
