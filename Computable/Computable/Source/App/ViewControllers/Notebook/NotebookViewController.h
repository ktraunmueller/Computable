//
//  NotebookViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "../Tabs/TabsViewController.h"

extern NSString *const NotebookWillExecuteCellNotification;
extern NSString *const InputCellDidBeginEditingNotification;
extern NSString *const InputCellDidEndEditingNotification;

@class Notebook;
@class InputCell;
@class HtmlCaptureView;

@interface NotebookViewController : UIViewController <TabsViewControllerChild>
{
@private
    __weak IBOutlet UICollectionView *_collectionView;
    __weak IBOutlet UIView *_toolbarView;
    __weak IBOutlet NSLayoutConstraint *_toolbarWidthConstraint;
    __weak IBOutlet NSLayoutConstraint *_toolbarFirstButtonTrailingConstraint;
    __weak IBOutlet UIButton *_toolbarRunButton;
    __weak IBOutlet UIButton *_toolbarRunFromHereButton;
    __weak IBOutlet UIButton *_toolbarInterruptButton;
    __weak IBOutlet UIButton *_toolbarEditButton;
    __weak IBOutlet UIButton *_toolbarCellTypeButton;
    __weak IBOutlet UIButton *_toolbarUpButton;
    __weak IBOutlet UIButton *_toolbarDownButton;
    __weak IBOutlet UIButton *_toolbarPlusButton;
    __weak IBOutlet UIButton *_toolbarMinusButton;
    __weak IBOutlet UIButton *_toolbarUndoButton;
    __weak IBOutlet UIButton *_toolbarRedoButton;
    __weak IBOutlet UIButton *_toolbarCloseButon;
    __weak IBOutlet NSLayoutConstraint *_toolbarTopConstraint;
    __weak IBOutlet NSLayoutConstraint *_toolbarTrailingConstraint;
}

@property (nonatomic, readonly) Notebook *notebook;
@property (nonatomic, weak) HtmlCaptureView *htmlCaptureView;

+ (NotebookViewController *)newInstanceWithNotebook:(Notebook *)notebook storyboard:(UIStoryboard *)storyboard;

- (IBAction)runCell:(id)sender;
- (IBAction)runFromCell:(id)sender;
- (IBAction)stopCell:(id)sender;
- (IBAction)editCell:(id)sender;
- (IBAction)toggleCellType:(id)sender;
- (IBAction)moveCellUp:(id)sender;
- (IBAction)moveCellDown:(id)sender;
- (IBAction)addCell:(id)sender;
- (IBAction)deleteCell:(id)sender;
- (IBAction)closeToolbar:(id)sender;

- (void)executeCell:(InputCell *)inputCell;
- (void)executeFromCell:(InputCell *)inputCell;

- (void)endEditing;

- (void)markSelectedRange;
- (void)restoreSelectedRange;
- (void)fileSelected:(NSString *)path;

@end
