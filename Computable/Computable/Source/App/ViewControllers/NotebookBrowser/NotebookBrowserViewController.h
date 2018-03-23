//
//  NotebookBrowserViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const NotebookBrowserDidSelectNotebookNotification;
extern NSString *const NotebookBrowserDidRequestNewNotebookNotification;
extern NSString *const NotebookBrowserWillDeleteNotebookNotification;

@interface NotebookBrowserViewController : UIViewController
{
@private
    __weak IBOutlet UICollectionView *_collectionView;
}

+ (NotebookBrowserViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard;

@end
