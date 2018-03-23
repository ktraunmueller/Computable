//
//  NotebookViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookViewController.h"
#import "InputCellView.h"
#import "OutputCellView.h"
#import "CompletionsViewController.h"
#import "../MainViewController.h"
#import "../Tabs/TabsViewController.h"
#import "../Tabs/TabView.h"
#import "../../Colors.h"
#import "../../Views/Text/Markdown/MarkdownEditorView.h"
#import "../../Views/Text/Python/PythonEditorView.h"
#import "../../Core/Analytics/Analytics.h"
#import "../../Core/IPython/IPythonApplication.h"
#import "../../Core/IPython/Kernel/Kernel.h"
#import "../../Core/IPython/Notebook/Notebook.h"
#import "../../Core/IPython/Notebook/Cells/Cell.h"
#import "../../Core/IPython/Notebook/Cells/InputCell.h"
#import "../../Core/IPython/Notebook/Cells/OutputCell.h"
#import "../../Core/Settings/Settings.h"
#import "../../Views/Html/HtmlCaptureView.h"
#import "../../../Common/Utilities/CALayer+Utils.h"
#import "../../../Common/Utilities/NSString+PathUtils.h"
#import "../../../Common/Utilities/UIImage+Resizing.h"

NSString *const NotebookWillExecuteCellNotification = @"NotebookWillExecuteCellNotification";
NSString *const InputCellDidBeginEditingNotification = @"InputCellDidBeginEditingNotification";
NSString *const InputCellDidEndEditingNotification = @"InputCellDidEndEditingNotification";

@interface NotebookViewController () <UICollectionViewDataSource, UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, TabsViewControllerChild, EditorViewDelegate>

@property (nonatomic, strong) Notebook *notebook;

@end

static const NSTimeInterval kToolbarFadeDuration = 0.25;
static const NSTimeInterval kToolbarSlideDuration = 0.25;

static const CGFloat ToolbarFirstButtonTrailingConstraintCode = 8;
static const CGFloat ToolbarFirstButtonTrailingConstraintText = -136;
static const CGFloat ToolbarWidthConstraintCode = 580 - 96; // undo/redo
static const CGFloat ToolbarWidthConstraintText = 436 - 96;

@implementation NotebookViewController
{
    BOOL _performedOneTimeSetup;
    BOOL _didShowScrollHint;
    BOOL _dirty;
    BOOL _didSaveThumbnail;
    __weak TabView *_tabView;
    PythonEditorView *_hiddenPythonFirstResponderView;
    MarkdownEditorView *_hiddenMarkdownFirstResponderView;
    UIInterfaceOrientation _interfaceOrientation;
    BOOL _prefersStatusBarHidden;
    BOOL _toolbarHidden;
    CGFloat _itemWidth; // TODO get rid of this
    BOOL _continueExecutingCells;
    BOOL _preventEndEditingOnKeyboardWillHide;
    NSRange _selectedRange;
}

#pragma mark - Statics

+ (NotebookViewController *)newInstanceWithNotebook:(Notebook *)notebook storyboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([NotebookViewController class]);
    NotebookViewController *notebookController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    notebookController.notebook = notebook;
    return notebookController;
}

#pragma mark - Properties

- (void)setNotebook:(Notebook *)notebook
{
    [self unregisterKVO];
    _notebook = notebook;
    [self registerKVO];
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"Notebook"];
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    _interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    _itemWidth = CGRectGetWidth(self.view.bounds);
    
    _collectionView.backgroundColor = [UIColor clearColor];
//    _collectionView.layer.borderWidth = 1;
//    _collectionView.layer.borderColor = [[UIColor orangeColor] CGColor];
    UICollectionViewFlowLayout *layout = (UICollectionViewFlowLayout *)_collectionView.collectionViewLayout;
    layout.sectionInset = UIEdgeInsetsZero;
    layout.minimumLineSpacing = 0;
    layout.minimumInteritemSpacing = 0;
    
    _toolbarUndoButton.hidden = YES;
    _toolbarRedoButton.hidden = YES;
    _toolbarView.clipsToBounds = YES;
    _toolbarHidden = YES;
    _toolbarView.alpha = 0;
    _toolbarTopConstraint.constant = 4;
    [self styleToolbar];
    
    _hiddenPythonFirstResponderView = [PythonEditorView newInstanceWithFrame:CGRectZero];
    _hiddenPythonFirstResponderView.editorDelegate = self;
    [self.view addSubview:_hiddenPythonFirstResponderView];
    _hiddenMarkdownFirstResponderView = [MarkdownEditorView newInstanceWithFrame:CGRectZero];
    _hiddenMarkdownFirstResponderView.editorDelegate = self;
    [self.view addSubview:_hiddenMarkdownFirstResponderView];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    _interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    _itemWidth = CGRectGetWidth(self.view.bounds);
    
    if (!_performedOneTimeSetup)
    {
        _performedOneTimeSetup = YES;
        if (![self renderMarkdown])
        {
            [self saveThumbnail];
        }
    }
    
    [self updateToolbarStateAndVisibility];
}

#pragma mark - NotebookViewController

- (void)dealloc
{
    [self unregisterCellKVO:_notebook.focusedInputCell];
    [self unregisterKVO];
    [self unregisterNotifications];
}

- (BOOL)renderMarkdown
{
    [_htmlCaptureView abortRendering];
    BOOL hasMarkdownCells = NO;
    for (Cell *cell in _notebook.cells)
    {
        if (![cell isKindOfClass:[InputCell class]])
        {
            continue;
        }
        InputCell *inputCell = (InputCell *)cell;
        if (!inputCell.isMarkdown)
        {
            continue;
        }
        hasMarkdownCells = YES;
        [self renderMarkdownCell:inputCell];
    }
    return hasMarkdownCells;
}

- (void)renderMarkdownCell:(InputCell *)inputCell
{
    CGFloat layoutWidth = CGRectGetWidth(self.view.frame) - InputCellViewHorizontalContentMargin;
    
    [_htmlCaptureView render:inputCell.markdownHtml
        waitForReadyCallback:inputCell.hasMath
                 layoutWidth:layoutWidth
                  completion:^(UIImage *image, NSArray *hyperlinks) {
                      inputCell.renderedMarkdownImage = image;
                      inputCell.hyperlinks = hyperlinks;
                      inputCell.cachedHeight = image.size.height;
                      NSUInteger index = [_notebook.cells indexOfObject:inputCell];
                      if (index != NSNotFound)
                      {
                          InputCellView *inputCellView = inputCell.context;
                          [inputCellView detachFromInputCell];
                          [_collectionView reloadItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:index inSection:0] ]];
                      }
                      if (!_didSaveThumbnail &&
                          UIInterfaceOrientationIsPortrait(_interfaceOrientation) &&
                          ([self topOffsetForCell:inputCell] > _collectionView.contentOffset.y + CGRectGetHeight(_collectionView.bounds) ||
                           [_notebook isLastMarkdownInputCell:inputCell]))
                      {
                          [self saveThumbnail];
                      }
                  }];
}

- (void)executeCell:(InputCell *)inputCell
{
    if (inputCell == nil || inputCell.source.length == 0)
    {
        [_tabView stopSpinner];
        _toolbarRunButton.enabled = YES;
        _toolbarRunFromHereButton.enabled = YES;
        _toolbarInterruptButton.enabled = NO;
        return;
    }
    
    if (inputCell.executing)
    {
        NSLog(@"cell already executing, ignoring");
        return;
    }
    
    NSRange youTubeRange = [inputCell.source rangeOfString:@"YouTubeVideo("];
    if (youTubeRange.location != NSNotFound)
    {
        NSUInteger startLocation = youTubeRange.location + youTubeRange.length;
        NSRange parenRange = [inputCell.source rangeOfString:@")" options:0 range:NSMakeRange(startLocation, inputCell.source.length - startLocation)];
        if (parenRange.location != NSNotFound)
        {
            NSString *videoId = [inputCell.source substringWithRange:NSMakeRange(startLocation + 1, parenRange.location - startLocation - 2)];
            NSString *url = [NSString stringWithFormat:@"https://www.youtube.com/watch?v=%@", videoId];
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
        }
        return;
    }
    
    NSUInteger inputCellIndex = [_notebook.cells indexOfObject:inputCell];
    if (inputCellIndex == NSNotFound)
    {
        return;
    }
    
    [_tabView startSpinner];
    _toolbarRunButton.enabled = NO;
    _toolbarRunFromHereButton.enabled = NO;
    _toolbarInterruptButton.enabled = YES;
    
    if (inputCell.dirty)
    {
        [_notebook save:YES];   // save input now
        _dirty = YES;           // set dirty flag to make sure we save latest output on close also
    }
    
    if (_continueExecutingCells)
    {
        _notebook.focusedInputCell.editing = NO;
    }
    
    [inputCell execute:^{
        if (inputCell.errorResponse && inputCell.executeReply == nil)
        {
            NSLog(@"received error response");
            _continueExecutingCells = NO;
            [_tabView stopSpinner];
            _toolbarRunButton.enabled = YES;
            _toolbarRunFromHereButton.enabled = YES;
            _toolbarInterruptButton.enabled = NO;
            
            if (inputCell.editing)
            {
                inputCell.preventEndEditingOnResignFirstResponder = YES;
                [self transferFirstResponderFrom:inputCell];
            }
            _preventEndEditingOnKeyboardWillHide = YES;
            [_collectionView performBatchUpdates:^{
                InputCellView *inputCellView = inputCell.context;
                [inputCellView detachFromInputCell];
                [_collectionView reloadItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:inputCellIndex inSection:0] ]];
            } completion:^(BOOL finished) {
                if (!finished)
                {
                    return;
                }
                _preventEndEditingOnKeyboardWillHide = NO;
            }];
            return;
        }
        
        if (inputCell.executeReply && inputCell.errorResponse == nil)
        {
            [_tabView stopSpinner];
            _toolbarRunButton.enabled = !_continueExecutingCells;
            _toolbarRunFromHereButton.enabled = !_continueExecutingCells;
            _toolbarInterruptButton.enabled = _continueExecutingCells;
            
            // update cell height now for proper toolbar positioning
            inputCell.cachedHeight = [InputCellView preferredHeightForCell:inputCell sourceEditing:inputCell.editing layoutWidth:_itemWidth];
            
            __block NSUInteger scrollIndex = NSNotFound;
            __block InputCell *newFocusedInputCell = nil;
            __block NSMutableArray *indexPathsToReload = [NSMutableArray array];
            
            [_collectionView performBatchUpdates:^{
                [indexPathsToReload addObject:[NSIndexPath indexPathForItem:inputCellIndex inSection:0]];
                if (inputCell.outputCell)
                {
                    NSUInteger outputCellIndex = [inputCell.outputCell.notebook.cells indexOfObject:inputCell.outputCell];
                    if (outputCellIndex != NSNotFound)
                    {
                        if ([inputCell.notebook.cells count] > [_collectionView numberOfItemsInSection:0])
                        {
                            // execute produced a new output cell
                            [_collectionView insertItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:outputCellIndex inSection:0] ]];
                        }
                        else
                        {
                            // reload existing output cell
                            [indexPathsToReload addObject:[NSIndexPath indexPathForItem:outputCellIndex inSection:0]];
                        }
                    }
                    // update output height now for proper toolbar positioning
                    inputCell.outputCell.cachedHeight = [OutputCellView preferredHeightForCell:inputCell.outputCell layoutWidth:_itemWidth];
                }
                
                if ([_notebook isLastInputCell:inputCell] && [inputCell.source length] > 0)
                {
                    // append new empty input cell
                    [self unregisterCellKVO:inputCell];
                    newFocusedInputCell = [_notebook newInputCell];
                    [self registerCellKVO:newFocusedInputCell];
                    
                    [self transferFirstResponderFrom:newFocusedInputCell];
                    _dirty = YES;
                    [_collectionView insertItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:_notebook.focusedInputCellIndex inSection:0] ]];
                    
                    [self scrollItemVisible:_notebook.focusedInputCellIndex];
                    [self updateToolbarStateAndVisibility];
                    [self updateToolbarPositionAnimated:YES];
                }
                else
                {
                    // focus next input cell
                    for (NSUInteger index = inputCellIndex + 1; index < [_notebook.cells count]; index++)
                    {
                        Cell *nextCell = [_notebook.cells objectAtIndex:index];
                        if ([nextCell isKindOfClass:[InputCell class]])
                        {
                            InputCell *nextInputCell = (InputCell *)nextCell;
                            if (nextInputCell.type == InputCellTypeCode)
                            {
                                newFocusedInputCell = nextInputCell;
                                scrollIndex = [_notebook.cells indexOfObject:newFocusedInputCell];
                                break;
                            }
                        }
                    }
                }
            } completion:^(BOOL finished) {
                if (!finished)
                {
                    return;
                }
                
                dispatch_block_t completion = ^{
                    if (newFocusedInputCell)
                    {
                        _notebook.focusedInputCell = newFocusedInputCell;
                        if (newFocusedInputCell.source.length == 0)
                        {
                            newFocusedInputCell.editing = YES;
                        }
                        if (_continueExecutingCells && !newFocusedInputCell.executing)
                        {
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [self executeCell:newFocusedInputCell];
                            });
                        }
                    }
                };
                
                if ([indexPathsToReload count] > 0)
                {
                    if (inputCell.editing)
                    {
                        inputCell.preventEndEditingOnResignFirstResponder = YES;
                        [self transferFirstResponderFrom:inputCell];
                    }
                    _preventEndEditingOnKeyboardWillHide = YES;
                    for (NSIndexPath *indexPath in indexPathsToReload)
                    {
                        Cell *cell = [_notebook.cells objectAtIndex:indexPath.item];
                        InputCellView *inputCellView = cell.context;
                        [inputCellView detachFromInputCell];
                    }
                    [_collectionView performBatchUpdates:^{
                        [_collectionView reloadItemsAtIndexPaths:indexPathsToReload];
                    } completion:^(BOOL finished) {
                        if (!finished)
                        {
                            return;
                        }
                        _preventEndEditingOnKeyboardWillHide = NO;
                        completion();
                    }];
                    return;
                }
                completion();
            }];
        }
    }];
}

- (void)executeFromCell:(InputCell *)inputCell
{
    _continueExecutingCells = YES;
    [self executeCell:inputCell];
}

- (void)interruptKernel
{
    _continueExecutingCells = NO;
    [_notebook interruptKernel];
    
    [_tabView stopSpinner];
    _toolbarRunButton.enabled = YES;
    _toolbarRunFromHereButton.enabled = YES;
    _toolbarInterruptButton.enabled = NO;
}

- (void)endEditing
{
    if (_notebook.focusedInputCell.editing)
    {
        _notebook.focusedInputCell.editing = NO;
    }
}

- (void)transferFirstResponderFrom:(InputCell *)cell
{
    if (cell.isMarkdown)
    {
        [_hiddenMarkdownFirstResponderView becomeFirstResponder];
    }
    else
    {
        [_hiddenPythonFirstResponderView becomeFirstResponder];
    }
}

- (void)scrollItemVisible:(NSUInteger)scrollIndex
{
    if (scrollIndex == NSNotFound || [_collectionView numberOfItemsInSection:0] == 0 || scrollIndex > [_collectionView numberOfItemsInSection:0] - 1)
    {
        return;
    }
    UICollectionViewLayoutAttributes *attributes = [_collectionView layoutAttributesForItemAtIndexPath:[NSIndexPath indexPathForItem:scrollIndex inSection:0]];
    CGRect cellFrame = attributes.frame;
    cellFrame.origin.y -= _collectionView.contentOffset.y;
    if (_notebook.focusedInputCellIndex == scrollIndex)
    {
        if (CGRectGetMinY(cellFrame) >= 0 && CGRectGetMaxY(cellFrame) <= CGRectGetHeight(_collectionView.frame) - _collectionView.contentInset.bottom)
        {
            // cell fully visible, don't scroll
            [self updateToolbarPositionAnimated:YES];
        }
        else
        {
            // scroll cell to top
            [_collectionView scrollToItemAtIndexPath:[NSIndexPath indexPathForItem:scrollIndex inSection:0]
                                    atScrollPosition:UICollectionViewScrollPositionTop
                                            animated:YES];
        }
    }
    else
    {
        if (CGRectGetMinY(cellFrame) < 0 || CGRectGetMinY(cellFrame) > CGRectGetHeight(_collectionView.frame))
        {
            // scroll cell into view only if out of visible range
            [_collectionView scrollToItemAtIndexPath:[NSIndexPath indexPathForItem:scrollIndex inSection:0]
                                    atScrollPosition:UICollectionViewScrollPositionTop
                                            animated:YES];
        }
    }
}

- (void)saveThumbnail
{
    _didSaveThumbnail = YES;
    
    CGFloat toolbarAlpha = _toolbarView.alpha;
    _toolbarView.alpha = 0;
    UIImage *screenshot = [self.view.layer captureViewIntoImage];
    _toolbarView.alpha = toolbarAlpha;
    
    screenshot = [screenshot resizedImageWithWidth:148];
    NSString *thumbnailPath = [IPythonApplication thumbnailPathForNotebook:_notebook.path];
    NSData *imageData = UIImagePNGRepresentation(screenshot);
    NSError *error = nil;
    if (![imageData writeToFile:thumbnailPath options:0 error:&error])
    {
        NSLog(@"failed to save thumbnail: %@", error);
    }
}

- (CGFloat)topOffsetForCell:(Cell *)aCell
{
    CGFloat top = 0;
    for (Cell *cell in _notebook.cells)
    {
        if (cell == aCell)
        {
            return top;
        }
        top += cell.cachedHeight;
    }
    return top;
}

- (void)showOrHideStatusBar
{
    _interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    _prefersStatusBarHidden = _notebook.focusedInputCell && _notebook.focusedInputCell.editing && UIInterfaceOrientationIsLandscape(_interfaceOrientation);
    MainViewController *mainViewController = (MainViewController *)self.parentViewController.parentViewController;
    [mainViewController hideStatusBar:_prefersStatusBarHidden];
}

- (void)styleToolbar
{
    _toolbarView.backgroundColor = [LightGrayBackground colorWithAlphaComponent:0.9];
    _toolbarView.layer.cornerRadius = 7;
    _toolbarView.layer.borderWidth = 1;
    _toolbarView.layer.borderColor = [[UIColor blackColor] CGColor];
    
    [self styleToolbarButton:_toolbarRunButton];
    [self styleToolbarButton:_toolbarRunFromHereButton];
    [self styleToolbarButton:_toolbarInterruptButton];
    [self styleToolbarButton:_toolbarEditButton];
    [self styleToolbarButton:_toolbarCellTypeButton];
    [self styleToolbarButton:_toolbarUpButton];
    [self styleToolbarButton:_toolbarDownButton];
    [self styleToolbarButton:_toolbarPlusButton];
    [self styleToolbarButton:_toolbarMinusButton];
    [self styleToolbarButton:_toolbarUndoButton];
    [self styleToolbarButton:_toolbarRedoButton];
    [self styleToolbarButton:_toolbarCloseButon];
    
    _toolbarCloseButon.contentMode = UIViewContentModeCenter;
    [_toolbarCloseButon setImage:[[UIImage imageNamed:@"Close"] resizedImageWithWidth:16]
                        forState:UIControlStateNormal];
}

- (void)styleToolbarButton:(UIButton *)button
{
    button.layer.cornerRadius = 6;
    button.backgroundColor = [UIColor clearColor];
}

- (void)updateToolbarStateAndVisibility
{
    InputCell *cell = _notebook.focusedInputCell;
    
    _toolbarRunButton.enabled = cell && cell.type == InputCellTypeCode;
    _toolbarRunFromHereButton.enabled = cell && cell.type == InputCellTypeCode;
    _toolbarInterruptButton.enabled = cell && cell.type == InputCellTypeCode;
    [_toolbarEditButton setImage:[UIImage imageNamed:cell.editing ? @"Check" : @"Edit"] forState:UIControlStateNormal];
    _toolbarEditButton.enabled = cell != nil;
    _toolbarCellTypeButton.enabled = YES;
    [_toolbarCellTypeButton setImage:[UIImage imageNamed:cell.type == InputCellTypeCode ? @"Console" : @"Text"] forState:UIControlStateNormal];
    _toolbarCellTypeButton.enabled = cell && cell.source.length == 0;
    _toolbarUpButton.enabled = cell && ![_notebook isFirstInputCell:cell];
    _toolbarDownButton.enabled = cell && ![_notebook isLastInputCell:cell];
    //    _toolbarUndoButton.enabled = [_notebook.undoManager canUndo];
    //    _toolbarRedoButton.enabled = [_notebook.undoManager canRedo];
    
    CGFloat toolbarWidth = cell.type == InputCellTypeCode ? ToolbarWidthConstraintCode : ToolbarWidthConstraintText;
    if (_toolbarWidthConstraint.constant != toolbarWidth)
    {
        _toolbarFirstButtonTrailingConstraint.constant = cell.type == InputCellTypeCode ? ToolbarFirstButtonTrailingConstraintCode : ToolbarFirstButtonTrailingConstraintText;
        _toolbarWidthConstraint.constant = toolbarWidth;
        [UIView animateWithDuration:kToolbarSlideDuration
                         animations:^{
                             [_toolbarView layoutIfNeeded];
                         }];
    }
    
    if (_toolbarHidden != (_toolbarView.alpha == 0))
    {
        [_toolbarView.layer removeAllAnimations];
        [UIView animateWithDuration:kToolbarFadeDuration
                              delay:0
                            options:UIViewAnimationOptionBeginFromCurrentState
                         animations:^{
                             _toolbarView.alpha = _toolbarHidden ? 0 : 1;
                         } completion:nil];
    }
}

- (void)updateToolbarPositionAnimated:(BOOL)animated
{
    if (_toolbarHidden)
    {
        return;
    }
    InputCell *focusedCell = _notebook.focusedInputCell;
    if (focusedCell == nil)
    {
        return;
    }
    
    CGFloat cellTop = [self topOffsetForCell:focusedCell];
    CGFloat toolbarHeight = CGRectGetHeight(_toolbarView.frame);
    CGFloat toolbarSpacing = 2;
    CGRect frame = _toolbarView.frame;
    if (cellTop < toolbarHeight + toolbarSpacing)
    {
        CGFloat cellHeight = [InputCellView preferredHeightForCell:focusedCell sourceEditing:focusedCell.editing layoutWidth:_itemWidth];
        _toolbarTopConstraint.constant = (focusedCell.isMarkdown && !focusedCell.editing ? 0 : cellTop + cellHeight) + toolbarSpacing;
    }
    else
    {
        _toolbarTopConstraint.constant = fmaxf(toolbarSpacing, cellTop - toolbarSpacing - toolbarHeight - _collectionView.contentOffset.y);
    }
    CGFloat maxY = CGRectGetHeight(self.view.frame) - _collectionView.contentInset.bottom - toolbarHeight - toolbarSpacing;
    if (_toolbarTopConstraint.constant > maxY)
    {
        _toolbarTopConstraint.constant = maxY; // don't position outside view
    }
    frame.origin.y = _toolbarTopConstraint.constant;
    if (!animated)
    {
        _toolbarView.frame = frame;
        return;
    }
    [UIView animateWithDuration:kToolbarSlideDuration
                     animations:^{
                         _toolbarView.frame = frame;
                         //                         [self.view layoutIfNeeded];    // Note: this triggers a full layout pass on the collection view! (don't use for scrolling)
                     }];
}

- (void)registerNotifications
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(inputCellSourceEmptyDidChange:)
                                                 name:InputCellSourceEmptyDidChangeNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(notebookCellHeightDidChange:)
                                                 name:NotebookCellHeightDidChangeNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(fontSettingsDidChange:)
                                                 name:FontSettingsDidChangeNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardDidShow:)
                                                 name:UIKeyboardDidShowNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillHide:)
                                                 name:UIKeyboardWillHideNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardDidHide:)
                                                 name:UIKeyboardDidHideNotification
                                               object:nil];
}

- (void)unregisterNotifications
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)registerKVO
{
    if (_notebook == nil)
    {
        return;
    }
    
//    NSLog(@"NotebookViewController %p registerKVO on %@", self, _notebook);
    
    [_notebook addObserver:self
                forKeyPath:@"focusedInputCell"
                   options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew
                   context:nil];
}

- (void)unregisterKVO
{
    if (_notebook == nil)
    {
        return;
    }
    
//    NSLog(@"NotebookViewController %p unregisterKVO on %@", self, _notebook);
    
    @try
    {
        [_notebook removeObserver:self forKeyPath:@"focusedInputCell"];
    }
    @catch (NSException * __unused exception)
    {
        // ignore
    }
}

- (void)registerCellKVO:(InputCell *)cell
{
//    NSLog(@"NotebookViewController %p registerCellKVO on %@", self, cell);
    
    [cell addObserver:self
           forKeyPath:@"editing"
              options:NSKeyValueObservingOptionNew
              context:nil];
}

- (void)unregisterCellKVO:(InputCell *)cell
{
    if (cell == nil || (id)cell == (id)[NSNull null])
    {
        return;
    }
    
//    NSLog(@"NotebookViewController %p unregisterKVO on %@", self, cell);
    
    @try
    {
        [cell removeObserver:self forKeyPath:@"editing"];
    }
    @catch (NSException * __unused exception)
    {
        // ignore
    }
}

- (void)endEditing:(BOOL)unfocus
{
    _notebook.focusedInputCell.editing = NO;
    _dirty |= _notebook.focusedInputCell.dirty;
    if (unfocus)
    {
        _toolbarHidden = YES;
        _notebook.focusedInputCell = nil;
    }
}

#pragma mark - IBActions

- (void)runCell:(id)sender
{
    _continueExecutingCells = NO;
    [self executeCell:_notebook.focusedInputCell];
}

- (void)runFromCell:(id)sender
{
    _continueExecutingCells = YES;
    [self executeCell:_notebook.focusedInputCell];
}

- (void)stopCell:(id)sender
{
    [self interruptKernel];
}

- (void)editCell:(id)sender
{
    _notebook.focusedInputCell.editing = !_notebook.focusedInputCell.editing;
}

- (void)toggleCellType:(id)sender
{
    if (_notebook.focusedInputCell == nil || _notebook.focusedInputCell.source.length > 0)
    {
        return;
    }
    if (_notebook.focusedInputCell.type == InputCellTypeCode && _notebook.focusedInputCell.outputCell)
    {
        NSUInteger index = [_notebook.cells indexOfObject:_notebook.focusedInputCell.outputCell];
        [_notebook deleteOutputCellForFocusedInputCell];
        if (index != NSNotFound)
        {
            [_collectionView deleteItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:index inSection:0] ]];
        }
    }
    _notebook.focusedInputCell.type = _notebook.focusedInputCell.type == InputCellTypeCode ? InputCellTypeMarkdown : InputCellTypeCode;
    _dirty = YES;
}

- (void)moveCellUp:(id)sender
{
    if (_notebook.focusedInputCellIndex == NSNotFound || _notebook.focusedInputCellIndex == 0)
    {
        return;
    }
    BOOL nextCellAlso = _notebook.focusedInputCell.outputCell != nil;
    NSUInteger index = _notebook.focusedInputCellIndex;
    NSUInteger newIndex = [_notebook moveFocusedCellUp];
    [self moveCellAtIndex:index toIndex:newIndex nextCellAlso:nextCellAlso];
}

- (void)moveCellDown:(id)sender
{
    if (_notebook.focusedInputCellIndex == NSNotFound || _notebook.focusedInputCellIndex == [_notebook.cells count] - 1)
    {
        return;
    }
    BOOL nextCellAlso = _notebook.focusedInputCell.outputCell != nil;
    NSUInteger index = _notebook.focusedInputCellIndex;
    NSUInteger newIndex = [_notebook moveFocusedCellDown];
    [self moveCellAtIndex:index toIndex:newIndex nextCellAlso:nextCellAlso];
}

- (void)moveCellAtIndex:(NSUInteger)index toIndex:(NSUInteger)newIndex nextCellAlso:(BOOL)nextCellAlso
{
    if (index == NSNotFound || newIndex == NSNotFound || index == newIndex)
    {
        return;
    }
    NSInteger numberOfItems = [_collectionView numberOfItemsInSection:0];
    // defensive check for https://rink.hockeyapp.net/manage/apps/61508/crash_reasons/16328916?no_iphone_ui=true
    if ((newIndex > numberOfItems - 1) || (nextCellAlso && newIndex + 1 > numberOfItems - 1))
    {
        return;
    }
    _dirty = YES;
    [_collectionView performBatchUpdates:^{
        NSIndexPath *fromIndexPath = [NSIndexPath indexPathForItem:index inSection:0];
        NSIndexPath *toIndexPath = [NSIndexPath indexPathForItem:newIndex inSection:0];
        [_collectionView moveItemAtIndexPath:fromIndexPath toIndexPath:toIndexPath];
        if (nextCellAlso)
        {
            fromIndexPath = [NSIndexPath indexPathForItem:index + 1 inSection:0];
            toIndexPath = [NSIndexPath indexPathForItem:newIndex + 1 inSection:0];
            [_collectionView moveItemAtIndexPath:fromIndexPath toIndexPath:toIndexPath];
        }
    } completion:^(BOOL finished) {
        if (!finished)
        {
            return;
        }
        [self updateToolbarStateAndVisibility];
        [self updateToolbarPositionAnimated:YES];
    }];
}

- (void)addCell:(id)sender
{
    BOOL wasEditing = _notebook.focusedInputCell.editing;
    if (_notebook.focusedInputCell.editing)
    {
        _notebook.focusedInputCell.editing = NO;
    }
    [self unregisterCellKVO:_notebook.focusedInputCell];
    if (_notebook.focusedInputCell == nil)
    {
        [_notebook newInputCell];
    }
    else
    {
        [_notebook newInputCellBelowFocusedCell];
    }
    [self registerCellKVO:_notebook.focusedInputCell];
    
    _dirty = YES;
    if (_notebook.focusedInputCellIndex == NSNotFound)
    {
        return;
    }
    if ([_notebook.cells count] > [_collectionView numberOfItemsInSection:0])
    {
        [_collectionView insertItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:_notebook.focusedInputCellIndex inSection:0] ]];
    }
    [self scrollItemVisible:_notebook.focusedInputCellIndex];
    if (wasEditing)
    {
        _notebook.focusedInputCell.editing = YES;
    }
    
    _toolbarHidden = NO;
    [self updateToolbarStateAndVisibility];
    [self updateToolbarPositionAnimated:YES];
}

- (void)deleteCell:(id)sender
{
    if (_notebook.focusedInputCell == nil)
    {
        return;
    }
    _dirty = YES;
    if (_notebook.focusedInputCell.editing)
    {
        _notebook.focusedInputCell.editing = NO;
    }
    NSMutableArray *indexPaths = [NSMutableArray arrayWithObjects:[NSIndexPath indexPathForItem:_notebook.focusedInputCellIndex inSection:0], nil];
    if (_notebook.focusedInputCell.outputCell)
    {
        NSUInteger outputIndex = [_notebook.cells indexOfObject:_notebook.focusedInputCell.outputCell];
        if (outputIndex != NSNotFound)
        {
            [indexPaths addObject:[NSIndexPath indexPathForItem:outputIndex inSection:0]];
        }
    }
    [_notebook deleteFocusedInputCell];
    [_collectionView deleteItemsAtIndexPaths:indexPaths];
    [self updateToolbarStateAndVisibility];
    [self updateToolbarPositionAnimated:YES];
}

- (void)closeToolbar:(id)sender
{
    _toolbarHidden = YES;
    [self updateToolbarStateAndVisibility];
}

- (void)markSelectedRange
{
    _selectedRange = _notebook.focusedInputCell.selectedRange;
}

- (void)restoreSelectedRange
{
    _notebook.focusedInputCell.selectedRange = _selectedRange;
}

- (void)fileSelected:(NSString *)path
{
    if (_notebook.focusedInputCell == nil)
    {
        return;
    }
    _dirty = YES;
    NSString *relativePath = [path stringByAbbreviatingWithTildeInPath];
    relativePath = [NSString stringWithFormat:@"'%@'", relativePath];
    NSString *source = relativePath;
    if (_notebook.focusedInputCell.source.length > 0)
    {
        source = [_notebook.focusedInputCell.source stringByReplacingCharactersInRange:_notebook.focusedInputCell.selectedRange withString:relativePath];
        _notebook.focusedInputCell.selectedRange = NSMakeRange(_notebook.focusedInputCell.selectedRange.location + relativePath.length, 0);
    }
    _notebook.focusedInputCell.source = source;
    InputCellView *inputCellView = _notebook.focusedInputCell.context;
    [inputCellView detachFromInputCell];
    [_collectionView reloadItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:_notebook.focusedInputCellIndex inSection:0] ]];
    _notebook.focusedInputCell.editing = YES;
}

#pragma mark - Notifications

- (void)inputCellSourceEmptyDidChange:(NSNotification *)notification
{
    Cell *cell = notification.object;
    if (cell.notebook != _notebook)
    {
        return;
    }
    [self updateToolbarStateAndVisibility];
}

- (void)notebookCellHeightDidChange:(NSNotification *)notification
{
    Cell *cell = notification.object;
    if (cell.notebook != _notebook)
    {
        return;
    }
    [self updateToolbarPositionAnimated:YES];

    // animated cell height change:
//    [_collectionView performBatchUpdates:nil completion:nil];
    
    // non-animated cell height change:
    [_collectionView.collectionViewLayout invalidateLayout];
    [_collectionView layoutIfNeeded];
}

- (void)notebookCellDidComputeHeight:(NSNotification *)notification
{
    Cell *cell = notification.object;
    if (cell.notebook != _notebook)
    {
        return;
    }
    if (cell)
    {
        NSUInteger index = [_notebook.cells indexOfObject:cell];
        if (index != NSNotFound)
        {
            InputCellView *inputCellView = cell.context;
            [inputCellView detachFromInputCell];
            [_collectionView reloadItemsAtIndexPaths:@[ [NSIndexPath indexPathForItem:index inSection:0] ]];
        }
    }
}

- (void)fontSettingsDidChange:(NSNotification *)notification
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [_notebook invalidateCells];
        [_collectionView reloadData];
        [self renderMarkdown];
    });
}

- (void)keyboardWillShow:(NSNotification *)notification
{
    if (!_tabView.active)
    {
        return;
    }
    [self showOrHideStatusBar];
    [self updateToolbarStateAndVisibility];
}

- (void)keyboardDidShow:(NSNotification *)notification
{
    if (!_tabView.active)
    {
        return;
    }
    CGSize keyboardSize = [[notification.userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    UIEdgeInsets contentInsets = UIEdgeInsetsMake(0.0, 0.0, fminf(keyboardSize.height, keyboardSize.width), 0.0);
    _collectionView.contentInset = contentInsets;
    _collectionView.scrollIndicatorInsets = contentInsets;
    
    if (_notebook.focusedInputCellIndex != NSNotFound)
    {
        [self scrollItemVisible:_notebook.focusedInputCellIndex];
        [self updateToolbarPositionAnimated:NO];
        
        if (!_didShowScrollHint)
        {
            _didShowScrollHint = YES;
            if (_notebook.focusedInputCell.isMarkdown)
            {
                [[MarkdownInputAccessoryView instance] showScrollHint];
            }
            else
            {
                [[PythonInputAccessoryView instance] showScrollHint];
            }
        }
    }
}

- (void)keyboardWillHide:(NSNotification *)notification
{
    if (!_tabView.active)
    {
        return;
    }
    UIEdgeInsets contentInsets = UIEdgeInsetsZero;
    _collectionView.contentInset = contentInsets;
    _collectionView.scrollIndicatorInsets = contentInsets;
    
    if (_notebook.focusedInputCell.editing && !_preventEndEditingOnKeyboardWillHide)
    {
        _notebook.focusedInputCell.editing = NO;
    }
}

- (void)keyboardDidHide:(NSNotification *)notification
{
}

#pragma mark - KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if (object == _notebook)
    {
        if ([keyPath isEqualToString:@"focusedInputCell"])
        {
            //            NSLog(@"focused input cell did change: %@ -> %@", change[NSKeyValueChangeOldKey], change[NSKeyValueChangeNewKey]);
            
            InputCell *oldFocusedInputCell = change[NSKeyValueChangeOldKey];
            [self unregisterCellKVO:oldFocusedInputCell];
            if (_notebook.focusedInputCell)
            {
                [self registerCellKVO:_notebook.focusedInputCell];
                
                _toolbarHidden = NO;
                [self updateToolbarStateAndVisibility];
                [self updateToolbarPositionAnimated:YES];
                [self scrollItemVisible:_notebook.focusedInputCellIndex];
            }
        }
        return;
    }
    
    if (object == _notebook.focusedInputCell)
    {
        if ([keyPath isEqualToString:@"editing"])
        {
            _dirty |= _notebook.focusedInputCell.dirty;
            
            if (!_notebook.focusedInputCell.editing)
            {
                [_collectionView performBatchUpdates:nil completion:nil];
                
                if (_notebook.focusedInputCell.isMarkdown && _notebook.focusedInputCell.dirty)
                {
                    [self renderMarkdownCell:_notebook.focusedInputCell];
                }
                
                [[NSNotificationCenter defaultCenter] postNotificationName:InputCellDidEndEditingNotification object:_notebook.focusedInputCell];
            }
            
            [self showOrHideStatusBar];
            
            if (_notebook.focusedInputCell.editing)
            {
                [[NSNotificationCenter defaultCenter] postNotificationName:InputCellDidBeginEditingNotification object:_notebook.focusedInputCell];
                
                _toolbarHidden = NO;
            }
            [self updateToolbarStateAndVisibility];
            if (!_notebook.focusedInputCell.editing)
            {
                [self updateToolbarPositionAnimated:YES];
            }
            return;
        }
    }
}

#pragma mark - TabsViewControllerChild

- (void)didMoveToTabsController:(TabsViewController *)tabsController tab:(TabView *)tab
{
    [self registerNotifications];
    
    _tabView = tab;
    _tabView.label.text = _notebook.title;
    
    if ([_notebook.cells count] == 0)
    {
        [self addCell:nil];
    }
}

- (void)willBeRemovedFromTabsController:(TabsViewController *)tabsController
{
    [self endEditing:NO];
    
    [self unregisterNotifications];
}

- (void)close:(dispatch_block_t)completion
{
    [self endEditing:YES];
    _notebook.focusedInputCell = nil;

    [_htmlCaptureView abortRendering];

    if (_dirty)
    {
        [_notebook save:YES];
        [self saveThumbnail];
    }

    [self showOrHideStatusBar];

    [_tabView startSpinner];
    [IPythonApplication closeNotebook:_notebook completion:^{
        [_tabView stopSpinner];
        completion();
    }];
}

#pragma mark - EditorViewDelegate

- (BOOL)editorViewWantsBecomeFirstResponder:(EditorView *)editorView
{
    return YES;
}

- (BOOL)editorViewWantsResignFirstResponder:(EditorView *)editorView
{
    return YES;
}

- (void)editorViewTextDidChange:(EditorView *)editorView
{
}

- (void)editorViewHeightDidChange:(EditorView *)editorView
{
}

#pragma mark - UICollectionViewDataSource

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return [_notebook.cells count];
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    Cell *cell = [_notebook.cells objectAtIndex:indexPath.item];
    if ([cell isKindOfClass:[InputCell class]])
    {
        InputCellView *cellView = (InputCellView *)[collectionView dequeueReusableCellWithReuseIdentifier:@"InputCellView" forIndexPath:indexPath];
        cellView.notebookController = self;
        cellView.cell = cell;
        return cellView;
    }
    if ([cell isKindOfClass:[OutputCell class]])
    {
        OutputCellView *cellView = (OutputCellView *)[collectionView dequeueReusableCellWithReuseIdentifier:@"OutputCellView" forIndexPath:indexPath];
        cellView.cell = cell;
        cellView.notebookController = self;
        return cellView;
    }
    return nil;
}

#pragma mark - UICollectionViewDelegateFlowLayout

- (CGSize)collectionView:(UICollectionView *)collectionView
                  layout:(UICollectionViewLayout *)collectionViewLayout
  sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    // make sure we're using the correct layout width at this point (viewWillAppear is not called for some reason)
    _interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    _itemWidth = CGRectGetWidth(self.view.bounds);
    
    Cell *cell = [_notebook.cells objectAtIndex:indexPath.item];
    if ([cell isKindOfClass:[InputCell class]])
    {
        InputCell *inputCell = (InputCell *)cell;
        CGSize size = CGSizeMake(_itemWidth, [InputCellView preferredHeightForCell:inputCell sourceEditing:inputCell.editing layoutWidth:_itemWidth]);
        cell.cachedHeight = size.height;
        return size;
    }
    if ([cell isKindOfClass:[OutputCell class]])
    {
        OutputCell *outputCell = (OutputCell *)cell;
        CGSize size = CGSizeMake(_itemWidth, [OutputCellView preferredHeightForCell:outputCell layoutWidth:_itemWidth]);
        cell.cachedHeight = size.height;
        return size;
    }
    return CGSizeZero;
}

#pragma mark - UIScrollViewDelegate

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    [self updateToolbarPositionAnimated:NO];
}

#pragma mark - Autorotation

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [self endEditing:NO];
    [_htmlCaptureView abortRendering];
    _interfaceOrientation = toInterfaceOrientation;
    [_collectionView.collectionViewLayout invalidateLayout];
    [[PythonInputAccessoryView instance] updateKeyboardLayoutForInterfaceOrientation:toInterfaceOrientation];
    
    [[CompletionsViewController instance] dismissAnimated:YES];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    _interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    _itemWidth = CGRectGetWidth(self.view.bounds);
    [self showOrHideStatusBar];
    [self updateToolbarStateAndVisibility];
    [_collectionView performBatchUpdates:nil completion:nil];
    [self renderMarkdown];
}

@end
