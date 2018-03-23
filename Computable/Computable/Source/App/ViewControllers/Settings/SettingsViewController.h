//
//  SettingsViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSUInteger, SettingsSection)
{
    SettingsSectionAppearance,
    SettingsSectionDropbox,
    SettingsSectionAbout,
    SettingsSectionNumberOfSections
};

typedef NS_ENUM(NSUInteger, SettingsSectionAppearanceItem)
{
    SettingsSectionAppearanceTextFont,
    SettingsSectionAppearanceCodeFont,
    SettingsSectionAppearanceNumberOfItems
};

typedef NS_ENUM(NSUInteger, SettingsSectionDropboxItem)
{
    SettingsSectionDropboxLink,
    SettingsSectionDropboxNumberOfItems
};

typedef NS_ENUM(NSUInteger, SettingsSectionAboutItem)
{
    SettingsSectionAboutAbout,
    SettingsSectionAboutRate,
    SettingsSectionAboutTour,
    SettingsSectionAboutRestoreInAppPurchase,
    SettingsSectionAboutNumberOfItems
};

@interface SettingsViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>
{
@private
    __weak IBOutlet UITableView *_tableView;
}

+ (SettingsViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard;

@end
