//
//  SettingsViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "SettingsViewController.h"
#import "FontSettingViewController.h"
#import "AboutViewController.h"
#import "DropboxViewController.h"
#import "../../AppDelegate.h"
#import "../../NSObject+App.h"
#import "../../Core/Analytics/Analytics.h"
#import "../../Core/Dropbox/Dropbox.h"
#import "../../Core/Settings/Settings.h"

static NSString *const SettingsCellIdentifier = @"SettingsCell";

static NSString *const kRateUrl = @"itms-apps://itunes.apple.com/app/id%@";

@implementation SettingsViewController
{
    NSString *_dropboxUsername;
}

#pragma mark - Statics

+ (SettingsViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([SettingsViewController class]);
    SettingsViewController *settingsController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    return settingsController;
}

#pragma mark - SettingsViewController

- (void)showTextFontSettings
{
    Settings *settings = [Settings instance];
    FontSettingViewController *fontSettingsController = [FontSettingViewController newInstanceFromStoryboard:self.storyboard
                                                                                                fontSettings:settings.availableTextFonts
                                                                                             selectedSetting:settings.textFont
                                                                                            selectionChanged:^(FontSetting *selection) {
                                                                                                settings.textFont = selection;
                                                                                                [settings save];
                                                                                            }];
    fontSettingsController.title = NSLocalizedString(@"Text Font", nil);
    [self.navigationController pushViewController:fontSettingsController animated:YES];
}

- (void)showCodeFontSettings
{
    Settings *settings = [Settings instance];
    FontSettingViewController *fontSettingsController = [FontSettingViewController newInstanceFromStoryboard:self.storyboard
                                                                                                fontSettings:settings.availableCodeFonts
                                                                                             selectedSetting:settings.codeFont
                                                                                            selectionChanged:^(FontSetting *selection) {
                                                                                                settings.codeFont = selection;
                                                                                                [settings save];
                                                                                            }];
    fontSettingsController.title = NSLocalizedString(@"Monospace Font", nil);
    [self.navigationController pushViewController:fontSettingsController animated:YES];
}

- (void)linkDropbox
{
    [Analytics trackEventWithCategory:@"Settings" action:@"show.dropbox" label:nil value:nil];
    
    DropboxViewController *dropboxController = [DropboxViewController newInstanceFromStoryboard:self.storyboard];
    [self.navigationController pushViewController:dropboxController animated:YES];
}

- (void)showAbout
{
    [Analytics trackEventWithCategory:@"Settings" action:@"show.about" label:nil value:nil];
    
    AboutViewController *aboutController = [AboutViewController newInstanceFromStoryboard:self.storyboard];
    [self.navigationController pushViewController:aboutController animated:YES];    
}

- (void)showInAppStore
{
    [Analytics trackEventWithCategory:@"Settings" action:@"rate" label:nil value:nil];
    
    NSString *appId = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"AppId"];
    NSString *url = [NSString stringWithFormat:kRateUrl, appId];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

- (void)showTour
{
    [self.presentingViewController dismissViewControllerAnimated:YES completion:^{
        [[NSNotificationCenter defaultCenter] postNotificationName:ShowTourRequestedNotification object:self];
    }];
}

- (void)done:(id)sender
{
    [self.presentingViewController dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
 
    [Analytics trackScreenView:@"Settings"];
    
    self.navigationItem.title = NSLocalizedString(@"Settings", nil);
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                           target:self
                                                                                           action:@selector(done:)];
    
    _tableView.dataSource = self;
    _tableView.delegate = self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    _dropboxUsername = [Dropbox username];
    
    [_tableView reloadData];
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section)
    {
        case SettingsSectionAppearance: return SettingsSectionAppearanceNumberOfItems;
        case SettingsSectionDropbox: return SettingsSectionDropboxNumberOfItems;
        case SettingsSectionAbout: return SettingsSectionAboutNumberOfItems;
        default:
            break;
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:SettingsCellIdentifier];
    static UIFont *defaultDetailFont = nil;
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:SettingsCellIdentifier];
        defaultDetailFont = cell.detailTextLabel.font;
    }
    cell.detailTextLabel.font = defaultDetailFont;
    cell.detailTextLabel.text = nil;
    cell.accessoryType = UITableViewCellAccessoryNone;
    
    Settings *settings = [Settings instance];
    
    switch (indexPath.section)
    {
        case SettingsSectionAppearance:
            switch (indexPath.row)
        {
            case SettingsSectionAppearanceTextFont:
                cell.textLabel.text = NSLocalizedString(@"Text Font", nil);
                cell.detailTextLabel.text = [NSString stringWithFormat:@"%@ %.0fpt", settings.textFont.displayName, settings.textFont.size];
                cell.detailTextLabel.font = [UIFont fontWithName:settings.textFont.regularFontName size:settings.textFont.size];
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                break;
            case SettingsSectionAppearanceCodeFont:
                cell.textLabel.text = NSLocalizedString(@"Monospace Font", nil);
                cell.detailTextLabel.text = [NSString stringWithFormat:@"%@ %.0fpt", settings.codeFont.displayName, settings.codeFont.size];
                cell.detailTextLabel.font = [UIFont fontWithName:settings.codeFont.regularFontName size:settings.codeFont.size];
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                break;
            default:
                break;
        }
            break;
            
        case SettingsSectionDropbox:
            switch (indexPath.row)
        {
            case SettingsSectionDropboxLink:
                cell.textLabel.text = _dropboxUsername ? [NSString stringWithFormat:NSLocalizedString(@"Linked to %@", nil), _dropboxUsername] : NSLocalizedString(@"Link with Dropbox", nil);
//                cell.detailTextLabel.text = _dropboxUsername ? NSLocalizedString(@"Unlink", nil) : nil;
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                break;
            default:
                break;
        }
            break;
            
        case SettingsSectionAbout:
            switch (indexPath.row)
        {
            case SettingsSectionAboutAbout:
                cell.textLabel.text = NSLocalizedString(@"About", nil);
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                break;
            case SettingsSectionAboutRate:
                cell.textLabel.text = NSLocalizedString(@"Rate", nil);
                cell.detailTextLabel.text = NSLocalizedString(@"Show in App Store", nil);
                cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
                break;
            case SettingsSectionAboutRestoreInAppPurchase:
                cell.textLabel.text = NSLocalizedString(@"In-app Purchases", nil);
                cell.detailTextLabel.text = NSLocalizedString(@"Restore", nil);
                break;
            case SettingsSectionAboutTour:
                cell.textLabel.text = NSLocalizedString(@"Tour", nil);
                cell.detailTextLabel.text = NSLocalizedString(@"Take the Tour", nil);
            default:
                break;
        }
            break;
            
        default:
            break;
    }
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return SettingsSectionNumberOfSections;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    switch (section)
    {
        case SettingsSectionAppearance: return NSLocalizedString(@"Appearance", nil);
        case SettingsSectionDropbox: return @"Dropbox";
            //        case SettingsSectionAbout: return NSLocalizedString(@"About", nil);
        default:
            break;
    }
    return nil;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    switch (indexPath.section)
    {
        case SettingsSectionAppearance:
            switch (indexPath.row)
        {
            case SettingsSectionAppearanceTextFont:
                [self showTextFontSettings];
                break;
            case SettingsSectionAppearanceCodeFont:
                [self showCodeFontSettings];
                break;
            default:
                break;
        }
            break;
            
        case SettingsSectionDropbox:
            switch (indexPath.row)
        {
            case SettingsSectionDropboxLink:
                [self linkDropbox];
                break;
            default:
                break;
        }
            break;
            
        case SettingsSectionAbout:
            switch (indexPath.row)
        {
            case SettingsSectionAboutAbout:
                [self showAbout];
                break;
            case SettingsSectionAboutRate:
                [self showInAppStore];
                break;
            case SettingsSectionAboutTour:
                [self showTour];
                break;
            case SettingsSectionAboutRestoreInAppPurchase:
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [self done:nil];
                });
                [self.appDelegate restoreInAppPurchases];
            }
                break;
            default:
                break;
        }
            break;
            
        default:
            break;
    }
}

@end
