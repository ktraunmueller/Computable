//
//  Settings.m
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Settings.h"

NSString *const FontSettingsDidChangeNotification = @"FontSettingsDidChangeNotification";

static NSString *const SettingsKeyTextFontName = @"TextFontName";
static NSString *const SettingsKeyTextFontSize = @"TextFontSize";
static NSString *const SettingsKeyCodeFontName = @"CodeFontName";
static NSString *const SettingsKeyCodeFontSize = @"CodeFontSize";

static CGFloat DefaultCodeFontSize = 12;
static CGFloat DefaultTextFontSize = 16;

@implementation Settings
{
    NSUserDefaults *_userDefaults;
    NSArray *_availableTextFonts;
    FontSetting *_selectedTextFont;
    NSArray *_availableCodeFonts;
    FontSetting *_selectedCodeFont;
}

#pragma mark - Statics

+ (Settings *)instance
{
    static Settings *instance = nil;
    if (instance == nil)
    {
        instance = [Settings new];
    }
    return instance;
}

#pragma mark - Properties

- (FontSetting *)textFont
{
    return _selectedTextFont;
}

- (void)setTextFont:(FontSetting *)textFont
{
    _selectedTextFont = textFont;
    [_userDefaults setObject:textFont.displayName forKey:SettingsKeyTextFontName];
    [_userDefaults setFloat:textFont.size forKey:SettingsKeyTextFontSize];
    [[NSNotificationCenter defaultCenter] postNotificationName:FontSettingsDidChangeNotification object:nil];
}

- (FontSetting *)codeFont
{
    return _selectedCodeFont;
}

- (void)setCodeFont:(FontSetting *)codeFont
{
    _selectedCodeFont = codeFont;
    [_userDefaults setObject:codeFont.displayName forKey:SettingsKeyCodeFontName];
    [_userDefaults setFloat:codeFont.size forKey:SettingsKeyCodeFontSize];
    [[NSNotificationCenter defaultCenter] postNotificationName:FontSettingsDidChangeNotification object:nil];
}

#pragma mark - Settings

- (id)init
{
    if (self = [super init])
    {
        _userDefaults = [NSUserDefaults standardUserDefaults];
        NSDictionary *defaults = @{ SettingsKeyTextFontName : @"Computer Modern",
                                    SettingsKeyTextFontSize : @(DefaultTextFontSize),
                                    SettingsKeyCodeFontName : @"DejaVu Sans Mono",
                                    SettingsKeyCodeFontSize : @(DefaultCodeFontSize) };
        [_userDefaults registerDefaults:defaults];
        
        // text font
        CGFloat textFontSize = [[_userDefaults objectForKey:SettingsKeyTextFontSize] floatValue];
        NSString *textFontName = [_userDefaults objectForKey:SettingsKeyTextFontName];

        _classicTextFont = [FontSetting new];
        _classicTextFont.displayName = @"Computer Modern";
        _classicTextFont.familyName = @"CMU Serif";
        _classicTextFont.regularFontName = @"CMUSerif-Roman";
        _classicTextFont.italicFontName = @"CMUSerif-Italic",
        _classicTextFont.boldFontName = @"CMUSerif-Bold";
        _classicTextFont.boldItalicFontName = @"CMUSerif-BoldItalic";
        _classicTextFont.headerFontName = @"CMUSerif-Roman";
        _classicTextFont.monospaceFamilyName = @"CMU Typewriter Text";
        
        _classicTextFont.sizes = @[ @(DefaultTextFontSize - 1), @(DefaultTextFontSize), @(DefaultTextFontSize + 1) ];
        _classicTextFont.size = textFontSize;
        
        _modernTextFont = [FontSetting new];
        _modernTextFont.displayName = @"Source Sans Pro";
        _modernTextFont.familyName = @"Source Sans Pro";
        _modernTextFont.regularFontName = @"SourceSansPro-Light";
        _modernTextFont.italicFontName = @"SourceSansPro-LightIt";
        _modernTextFont.boldFontName = @"SourceSansPro-Regular";
        _modernTextFont.boldItalicFontName = @"SourceSansPro-It";
        _modernTextFont.headerFontName = @"SourceSansPro-Semibold";
        _modernTextFont.monospaceFamilyName = @"DejaVu Sans Mono";
                
        _modernTextFont.sizes = @[ @(DefaultTextFontSize - 1), @(DefaultTextFontSize), @(DefaultTextFontSize + 1) ];
        _modernTextFont.size = DefaultTextFontSize;

        _availableTextFonts = @[ _classicTextFont, _modernTextFont ];
        _selectedTextFont = [self fontNamed:textFontName inFontSettings:_availableTextFonts];
        
        // code font
        CGFloat codeFontSize = [[_userDefaults objectForKey:SettingsKeyCodeFontSize] floatValue];
        NSString *codeFontName = [_userDefaults objectForKey:SettingsKeyCodeFontName];
        
        FontSetting *defaultCodeFont = [FontSetting new];
        defaultCodeFont.displayName = @"DejaVu Sans Mono";
        defaultCodeFont.familyName = @"DejaVu Sans Mono";
        defaultCodeFont.regularFontName = @"DejaVuSansMono";
        defaultCodeFont.italicFontName = @"DejaVuSansMono-Oblique";
        defaultCodeFont.boldFontName = @"DejaVuSansMono-Bold";
        defaultCodeFont.boldItalicFontName = @"DejaVuSansMono-BoldOblique";
        
        defaultCodeFont.sizes = @[ @(DefaultCodeFontSize - 1), @(DefaultCodeFontSize), @(DefaultCodeFontSize + 1) ];
        defaultCodeFont.size = codeFontSize;
        
        _availableCodeFonts = @[ defaultCodeFont ];
        _selectedCodeFont = [self fontNamed:codeFontName inFontSettings:_availableCodeFonts];
    }
    return self;
}

- (void)save
{
    [_userDefaults synchronize];
}

- (FontSetting *)fontNamed:(NSString *)displayName inFontSettings:(NSArray *)fontSettings
{
    for (FontSetting *fontSetting in fontSettings)
    {
        if ([fontSetting.displayName isEqualToString:displayName])
        {
            return fontSetting;
        }
    }
    return nil;
}

@end
