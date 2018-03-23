//
//  FontSettingViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "FontSettingViewController.h"
#import "../../Core/Analytics/Analytics.h"

static NSString *const FontSettingCellIdentifier = @"FontSettingCell";

@interface FontSettingViewController ()

@property (nonatomic, strong) NSArray *fontSettings;
@property (nonatomic, strong) FontSetting *selectedSetting;
@property (nonatomic, strong) void (^selectionChanged)(FontSetting *selectedFont);

@end

@implementation FontSettingViewController
{
    NSIndexPath *_selectedIndexPath;
}

#pragma mark - Statics

+ (FontSettingViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
                                            fontSettings:(NSArray *)fontSettings
                                         selectedSetting:(FontSetting *)selectedSetting
                                        selectionChanged:(void (^)(FontSetting *selection))selectionChanged
{
    NSString *identifier = NSStringFromClass([FontSettingViewController class]);
    FontSettingViewController *fontSettingController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    fontSettingController.fontSettings = fontSettings;
    fontSettingController.selectedSetting = selectedSetting;
    fontSettingController.selectionChanged = selectionChanged;
    return fontSettingController;
}

#pragma mark - FontSettingViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Font Settings"];
    
    _tableView.dataSource = self;
    _tableView.delegate = self;
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    FontSetting *setting = [_fontSettings objectAtIndex:section];
    return [setting.sizes count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:FontSettingCellIdentifier];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:FontSettingCellIdentifier];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    FontSetting *fontSetting = [_fontSettings objectAtIndex:indexPath.section];
    
    CGFloat fontSize = [[fontSetting.sizes objectAtIndex:indexPath.row] floatValue];
    cell.textLabel.text = [NSString stringWithFormat:@"%@ %.0fpt", fontSetting.displayName, fontSize];
    cell.textLabel.font = [UIFont fontWithName:fontSetting.regularFontName size:fontSize];
    cell.accessoryType = fontSetting == _selectedSetting && fontSize == fontSetting.size ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    
    if (cell.accessoryType == UITableViewCellAccessoryCheckmark)
    {
        _selectedIndexPath = indexPath;
    }
    
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [_fontSettings count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    FontSetting *setting = [_fontSettings objectAtIndex:section];
    return setting.displayName;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if (_selectedIndexPath)
    {
        UITableViewCell *previouslySelectedCell = [tableView cellForRowAtIndexPath:_selectedIndexPath];
        previouslySelectedCell.accessoryType = UITableViewCellAccessoryNone;
    }
    _selectedIndexPath = indexPath;
    UITableViewCell *newSelectedCell = [tableView cellForRowAtIndexPath:_selectedIndexPath];
    newSelectedCell.accessoryType = UITableViewCellAccessoryCheckmark;
    
    FontSetting *newSelection = [_fontSettings objectAtIndex:indexPath.section];
    newSelection.size = [[newSelection.sizes objectAtIndex:indexPath.row] floatValue];
    if (_selectionChanged)
    {
        _selectionChanged(newSelection);
    }
}

@end
