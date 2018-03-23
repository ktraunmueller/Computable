//
//  FontSettingViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "../../Core/Settings/FontSetting.h"

#import <UIKit/UIKit.h>

@interface FontSettingViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>
{
@private
    __weak IBOutlet UITableView *_tableView;
}

+ (FontSettingViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
                                            fontSettings:(NSArray *)fontSettings
                                         selectedSetting:(FontSetting *)selectedSetting
                                        selectionChanged:(void (^)(FontSetting *selection))selectionChanged;

@end
