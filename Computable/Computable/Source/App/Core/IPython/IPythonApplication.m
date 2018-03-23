//
//  IPythonApplication.m
//  Computable
//
//  Created by Karl Traunmüller on 02.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "IPythonApplication.h"
#import "Kernel/Kernel.h"
#import "Notebook/Notebook.h"
#import "Session/Session.h"
#import "Session/Message.h"
#import "../Analytics/Analytics.h"
#import "../Python/Runtime/PythonRuntime.h"
#import "../../../Common/Utilities/Filesystem.h"
#import <Python.h>

NSString *const NotebookApplicationDidStartNotification = @"NotebookApplicationDidStartNotification";
NSString *const NotebookApplicationDidStopNotification = @"NotebookApplicationDidStopNotification";

NSString *const kNotebookExtension = @"ipynb";
static NSString *const kNotebookThumbnailExtension = @"png";
static NSString *const kCategoriesFilename = @"categories.plist";
static NSString *const kCategoryComputable = @"Computable";
static NSString *const kCategoryMyNotebooks = @"My Notebooks";
static NSString *const kCategoryExamples = @"Examples";

static NSString *_profileDir;
static NSString *_notebooksDir;
static NSMutableArray *_openNotebooks;
static NSMutableDictionary *_notebookFilesByCategory;

static PyObject *_globals;
static PyObject *_notebookAppObj;
static PyObject *_kernelAppObj;
static PyObject *_kernelManager;
static PyObject *_notebookManager;

@implementation IPythonApplication

#pragma mark - Statics

+ (void)prepare:(dispatch_block_t)completion
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSError *error = nil;
        
        // ipython
        NSString *cachesDir = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
        _profileDir = [cachesDir stringByAppendingPathComponent:@"ipython/profile_default"];
        [fileManager createDirectoryAtPath:_profileDir
               withIntermediateDirectories:YES
                                attributes:Nil
                                     error:NULL];
        
        // notebooks
        NSString *documentsDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
        _notebooksDir = [documentsDir stringByAppendingPathComponent:@"notebooks"];
        if (![fileManager fileExistsAtPath:_notebooksDir])
        {
            NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
            NSString *bundledNotebooks = [resourcePath stringByAppendingPathComponent:@"notebooks"];
            [fileManager copyItemAtPath:bundledNotebooks
                                 toPath:_notebooksDir
                                  error:&error];
            NSArray *bundldedFiles = [fileManager contentsOfDirectoryAtPath:_notebooksDir error:NULL];
            for (NSString *bundledFile in bundldedFiles)
            {
                [Filesystem addSkipBackupAttributeToItemAtPath:[_notebooksDir stringByAppendingPathComponent:bundledFile]];
            }
        }
        
        // matplotlib
        NSString *documentsFolder = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
        NSString *mplConfigFolder = [documentsFolder stringByAppendingPathComponent:@"matplotlib/config"];
        [fileManager createDirectoryAtPath:mplConfigFolder
               withIntermediateDirectories:YES
                                attributes:nil
                                     error:NULL];
        NSString *matplotlibrcPath = [mplConfigFolder stringByAppendingPathComponent:@"matplotlibrc"];
        if (![fileManager fileExistsAtPath:matplotlibrcPath])
        {
            BOOL retina = [UIScreen mainScreen].scale == 2;
            NSString *matplotlibrcBundleFile = retina ? @"matplotlibrc-retina" : @"matplotlibrc";
            NSString *matplotlibrcBundlePath = [[NSBundle mainBundle] pathForResource:matplotlibrcBundleFile ofType:nil];
            [fileManager copyItemAtPath:matplotlibrcBundlePath
                                 toPath:matplotlibrcPath
                                  error:NULL];
            [Filesystem addSkipBackupAttributeToItemAtPath:matplotlibrcPath];
        }
        NSString *cachesFolder = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
        NSString *mplFontCache = [cachesFolder stringByAppendingPathComponent:@"matplotlib/cache"];
        [fileManager createDirectoryAtPath:mplFontCache
               withIntermediateDirectories:YES
                                attributes:nil
                                     error:NULL];
        
        NSString *categoriesFile = [_notebooksDir stringByAppendingPathComponent:kCategoriesFilename];
        _notebookFilesByCategory = [[NSDictionary dictionaryWithContentsOfFile:categoriesFile] mutableCopy];
        
        [IPythonApplication reloadNotebookFiles];
        
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion();
            });
        }
    });
}

#pragma mark - Lifecycle

+ (void)start:(dispatch_block_t)completion
{
    _openNotebooks = [NSMutableArray array];
    
    [[PythonRuntime instance] detachThread:^{
        NSLog(@"Starting IPython application...");
        
        _globals = PyDict_New();
        
        CFAbsoluteTime start = CFAbsoluteTimeGetCurrent();
        PyObject *appModuleObj = PyImport_ImportModule("IPython.embedded.notebookapp");
        PyMapping_SetItemString(_globals, "notebookapp", appModuleObj);
        Py_XDECREF(appModuleObj);
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"importing IPython application took %d ms", (int)((CFAbsoluteTimeGetCurrent() - start) * 1000));
        });
        
        start = CFAbsoluteTimeGetCurrent();
        _notebookAppObj = PyRun_StringFlags("notebookapp.NotebookApp.instance()", Py_eval_input, _globals, _globals, NULL);
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"instantiating IPython application took %d ms", (int)((CFAbsoluteTimeGetCurrent() - start) * 1000));
        });
        if (!_notebookAppObj)
        {
            NSLog(@"could not create IPython application");
            return;
        }
        
        start = CFAbsoluteTimeGetCurrent();
        PyObject *initialize = PyObject_GetAttrString(_notebookAppObj, "initialize");
        PyObject *argv = PyList_New(4);
        // TODO these strings are probably never released
        PyList_SetItem(argv, 0, PyString_FromString("--notebook-dir"));
        PyList_SetItem(argv, 1, PyString_FromString([_notebooksDir UTF8String]));
        PyList_SetItem(argv, 2, PyString_FromString("--profile-dir"));
        PyList_SetItem(argv, 3, PyString_FromString([_profileDir UTF8String]));
        PyObject *tuple = PyTuple_New(1);
        PyTuple_SetItem(tuple, 0, argv);
        PyObject_CallObject(initialize, tuple);
        Py_XDECREF(tuple);
        Py_XDECREF(argv);
        Py_XDECREF(initialize);
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"initializing IPython application took %d ms", (int)((CFAbsoluteTimeGetCurrent() - start) * 1000));
        });
        
        _kernelManager = PyObject_GetAttrString(_notebookAppObj, "kernel_manager");
        _notebookManager = PyObject_GetAttrString(_notebookAppObj, "notebook_manager");
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (completion)
            {
                completion();
            }
        });
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:NotebookApplicationDidStartNotification object:nil];
        });
        
        start = CFAbsoluteTimeGetCurrent();
        PyObject_CallMethod(_notebookAppObj, "start", NULL);
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"starting IPython application took %d ms", (int)((CFAbsoluteTimeGetCurrent() - start) * 1000));
        });
        NSLog(@"IPython application started");
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (completion)
            {
                completion();
            }
        });
    }];
}

+ (void)stop:(dispatch_block_t)completion
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:NotebookApplicationDidStopNotification object:nil];
    });
    
    [[PythonRuntime instance] execute:^{
        PyObject_CallMethod(_notebookAppObj, "stop", NULL);
        
        Py_XDECREF(_notebookManager); _notebookManager = nil;
        Py_XDECREF(_kernelManager); _kernelManager = nil;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"IPython application stopped");
            if (completion)
            {
                completion();
            }
            [[NSNotificationCenter defaultCenter] postNotificationName:NotebookApplicationDidStopNotification object:self];
        });
    }];
}

+ (void)doOneIteration:(id)kernel
{
    [[PythonRuntime instance] execute:^{
        PyObject_CallMethod(_kernelAppObj, "do_one_iteration", NULL);
        PyGC_Collect();
    }];
}

+ (void)interruptKernel:(Kernel *)kernel
{
    [[PythonRuntime instance] execute:^{
        PyObject *kernelIdObj = PyString_FromString([kernel.kernelId UTF8String]);
        PyObject *tuple = PyTuple_New(1);
        PyTuple_SetItem(tuple, 0, kernelIdObj);
        PyObject_CallObject(PyObject_GetAttrString(_kernelManager, "interrupt_kernel"), tuple);
        Py_XDECREF(kernelIdObj);
        Py_XDECREF(tuple);
    }];
}

#pragma mark - Notebook Files

+ (void)reloadNotebookFiles
{
    NSMutableArray *notebookFiles = [NSMutableArray array];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;
    NSArray *filesOrFolders = [fileManager contentsOfDirectoryAtPath:_notebooksDir error:&error];
    for (NSString *fileOrFolder in filesOrFolders)
    {
        if ([fileOrFolder.pathExtension isEqualToString:kNotebookExtension])
        {
            [notebookFiles addObject:fileOrFolder];
        }
    }
    [notebookFiles sortUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        NSString *file1 = obj1;
        NSString *file2 = obj2;
        return [file1 compare:file2 options:NSCaseInsensitiveSearch];
    }];
    
    NSMutableArray *myNotebooks = [[_notebookFilesByCategory objectForKey:kCategoryMyNotebooks] mutableCopy];
    for (NSString *notebook in notebookFiles)
    {
        NSString *category = [IPythonApplication categoryForNotebook:notebook];
        if (category == nil)
        {
            [myNotebooks addObject:notebook];
        }
    }
    [_notebookFilesByCategory setObject:myNotebooks forKey:kCategoryMyNotebooks];
    
    [IPythonApplication saveCategories];
}

+ (NSArray *)notebookCategories
{
    return @[ kCategoryMyNotebooks, kCategoryComputable, kCategoryExamples ];
}

+ (NSArray *)notebooksInCategory:(NSString *)category
{
    NSMutableArray *notebooks = [[_notebookFilesByCategory objectForKey:category] mutableCopy];
    [notebooks sortUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        NSString *file1 = obj1;
        NSString *file2 = obj2;
        return [file1 compare:file2 options:NSCaseInsensitiveSearch];
    }];
    return notebooks;
}

+ (NSString *)categoryForNotebook:(NSString *)notebookFile
{
    if (notebookFile == nil)
    {
        return nil;
    }
    __block NSString *result = nil;
    [_notebookFilesByCategory enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
        NSString *category = key;
        NSArray *files = obj;
        for (NSString *file in files)
        {
            if ([file compare:notebookFile options:NSCaseInsensitiveSearch] == NSOrderedSame)
            {
                *stop = YES;
                result = category;
                break;
            }
        }
    }];
    return result;
}

+ (void)saveCategories
{
    NSString *categoriesFile = [_notebooksDir stringByAppendingPathComponent:kCategoriesFilename];
    [_notebookFilesByCategory writeToFile:categoriesFile atomically:YES];
}

+ (BOOL)allowEditingCategory:(NSString *)category
{
    return [category isEqualToString:kCategoryMyNotebooks];
}

+ (NSString *)renameNotebookFromFile:(NSString *)fromFile toFile:(NSString *)toFile
{
    if (fromFile == nil || toFile.length == 0)
    {
        return fromFile;
    }
    if (![[toFile pathExtension] isEqualToString:kNotebookExtension])
    {
        toFile = [toFile stringByAppendingPathExtension:kNotebookExtension];
    }
    if (toFile == nil)
    {
        return fromFile;
    }
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *fromPath = [_notebooksDir stringByAppendingPathComponent:fromFile];
    NSString *toPath = [_notebooksDir stringByAppendingPathComponent:toFile];
    NSError *error = nil;
    [fileManager moveItemAtPath:fromPath
                         toPath:toPath
                          error:&error];
    if (error)
    {
        NSLog(@"%@", error);
    }
    
    NSString *category = [IPythonApplication categoryForNotebook:fromFile];
    NSMutableArray *filesInCategory = [_notebookFilesByCategory objectForKey:category == nil ? kCategoryMyNotebooks : category];
    [filesInCategory removeObject:fromFile];
    [filesInCategory addObject:toFile];
    [IPythonApplication saveCategories];
    
    NSString *thumbnailPath = [[fromPath stringByDeletingPathExtension] stringByAppendingPathExtension:kNotebookThumbnailExtension];
    NSString *newThumbnailPath = [[toPath stringByDeletingPathExtension] stringByAppendingPathExtension:kNotebookThumbnailExtension];
    error = nil;
    [fileManager moveItemAtPath:thumbnailPath
                         toPath:newThumbnailPath
                          error:&error];
    
    [IPythonApplication reloadNotebookFiles];
    
    return toFile;
}

+ (BOOL)deleteNotebook:(NSString *)notebookFile
{
    if (notebookFile.length == 0)
    {
        return NO; // https://rink.hockeyapp.net/manage/apps/61508/app_versions/44/crash_reasons/17141751?type=crashes
    }
    
    NSString *category = [IPythonApplication categoryForNotebook:notebookFile];
    NSMutableArray *filesInCategory = [_notebookFilesByCategory objectForKey:category == nil ? kCategoryMyNotebooks : category];
    [filesInCategory removeObject:notebookFile];
    [self saveCategories];
    
    NSString *thumbnailPath = [IPythonApplication thumbnailPathForNotebook:notebookFile];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;
    [fileManager removeItemAtPath:thumbnailPath error:&error];
    if (error)
    {
        NSLog(@"error deleting thumbanil: %@", error);
    }
    
    if ([[notebookFile pathExtension] length] == 0)
    {
        notebookFile = [notebookFile stringByAppendingPathExtension:kNotebookExtension];
    }
    NSString *notebookPath = notebookFile;
    if (![notebookPath isAbsolutePath])
    {
        notebookPath = [_notebooksDir stringByAppendingPathComponent:notebookPath];
    }
    error = nil;
    [fileManager removeItemAtPath:notebookPath error:&error];
    if (error)
    {
        NSLog(@"error deleting notebook: %@", error);
    }
    return error == nil;
}

+ (NSString *)thumbnailPathForNotebook:(NSString *)notebookFile
{
    NSString *baseFilename = [[notebookFile lastPathComponent] stringByDeletingPathExtension];
    return [_notebooksDir stringByAppendingPathComponent:[baseFilename stringByAppendingPathExtension:kNotebookThumbnailExtension]];
}

#pragma mark - Notebooks

+ (void)newNotebook:(void (^)(Notebook *))completion
{
    [[PythonRuntime instance] execute:^{
        PyObject *notebookIdObj = PyObject_CallMethod(_notebookManager, "new_notebook", NULL);
        [IPythonApplication startKernelForNotebookId:notebookIdObj notebookName:NULL completion:completion];
    }];
}

+ (void)openNotebook:(NSString *)notebookPath completion:(void (^)(Notebook *))completion
{
    [[PythonRuntime instance] execute:^{
        NSString *notebookFile = [notebookPath lastPathComponent];
        PyObject *notebookNameObj = PyString_FromString([notebookFile UTF8String]);
        
        PyObject *tuple = PyTuple_New(1);
        PyTuple_SetItem(tuple, 0, notebookNameObj);
        PyObject *notebookIdObj = PyObject_CallObject(PyObject_GetAttrString(_notebookManager, "new_notebook_id"), tuple);
        Py_XDECREF(notebookNameObj);
        Py_XDECREF(tuple);
        
        [IPythonApplication startKernelForNotebookId:notebookIdObj notebookName:[notebookFile UTF8String] completion:completion];
    }];
}

+ (NSString *)copyExternalNotebook:(NSURL *)notebookUrl
{
    if (notebookUrl == nil || ![[notebookUrl.pathExtension lowercaseString] isEqualToString:kNotebookExtension])
    {
        return nil;
    }
    
    NSString *notebookFile = notebookUrl.lastPathComponent;
    NSRange dotRange = [notebookFile rangeOfString:@"." options:NSBackwardsSearch];
    if (dotRange.location != NSNotFound)
    {
        NSRange dashRange = [notebookFile rangeOfString:@"-" options:NSBackwardsSearch];
        if (dashRange.location != NSNotFound && dashRange.location < dotRange.location)
        {
            NSString *suffix = [notebookFile substringWithRange:NSMakeRange(dashRange.location + 1, dotRange.location - dashRange.location - 1)];
            NSInteger number = [suffix intValue];
            if (number > 0)
            {
                notebookFile = [[notebookFile substringToIndex:dashRange.location] stringByAppendingPathExtension:kNotebookExtension];
            }
        }
    }
    
    [Analytics trackEventWithCategory:@"Notebook" action:@"copy.external" label:notebookFile value:nil];
    
    NSString *localPath = [_notebooksDir stringByAppendingPathComponent:notebookFile];
    if (localPath == nil)
    {
        return nil;
    }
    NSURL *localUrl = [NSURL fileURLWithPath:localPath];
    NSError *error = nil;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:localPath isDirectory:NULL])
    {
        [fileManager removeItemAtPath:localPath error:&error];
        if (error)
        {
            NSLog(@"error deleting notebook: %@", error);
        }
    }
    [fileManager copyItemAtURL:notebookUrl
                         toURL:localUrl
                         error:&error];
    if (error)
    {
        NSLog(@"error copying notebook: %@", error);
        return nil;
    }
    
    [IPythonApplication reloadNotebookFiles];
    return localPath;;
}

+ (void)startKernelForNotebookId:(PyObject *)notebookIdObj notebookName:(const char *)notebookNameCStr completion:(void (^)(Notebook *))completion
{
    PyObject *tuple = PyTuple_New(1);
    if (notebookNameCStr == NULL)
    {
        PyTuple_SetItem(tuple, 0, notebookIdObj);
        PyObject *notebookNameObj = PyObject_CallObject(PyObject_GetAttrString(_notebookManager, "get_name"), tuple);
        notebookNameCStr = notebookNameObj ? PyString_AsString(notebookNameObj) : NULL;
        Py_XDECREF(tuple);
    }
    
    char *notebookIdCStr = PyString_AsString(notebookIdObj);
    NSString *notebookdId = [NSString stringWithUTF8String:notebookIdCStr];
    
    tuple = PyTuple_New(1);
    PyTuple_SetItem(tuple, 0, notebookIdObj);
    Py_XDECREF(notebookIdObj);
    PyObject *kernelIdObj = PyObject_CallObject(PyObject_GetAttrString(_kernelManager, "start_kernel"), tuple);
    Py_XDECREF(tuple);
    char *kernelIdCStr = kernelIdObj ? PyString_AsString(kernelIdObj) : NULL;
    NSString *kernelId = kernelIdCStr ? [NSString stringWithUTF8String:kernelIdCStr] : nil;
    Py_XDECREF(kernelIdObj);
    Py_XDECREF(notebookIdObj);
    if (kernelId == nil)
    {
        NSLog(@"could not start kernel");
        return;
    }
    
    NSString *connectionFilePath = [IPythonApplication connectionFilePathForKernelId:kernelId];
    NSDictionary *connectionConfiguration = [IPythonApplication connectionConfigurationFromFile:connectionFilePath];
    
    Session *session = [[Session alloc] initWithId:kernelId configuration:connectionConfiguration];
    Kernel *kernel = [Kernel newInstanceWithId:kernelId session:session];
    [session start];
    
    NSString *notebookFilename = [NSString stringWithUTF8String:notebookNameCStr];
    if ([notebookFilename pathExtension].length == 0)
    {
        notebookFilename = [notebookFilename stringByAppendingPathExtension:kNotebookExtension];
    }
    NSString *notebookPath = [_notebooksDir stringByAppendingPathComponent:notebookFilename];
    Notebook *notebook = [Notebook newInstanceWithKernel:kernel path:notebookPath notebookId:notebookdId];
    notebook.title = [notebookFilename stringByDeletingPathExtension];
    [_openNotebooks addObject:notebook];
    
    // create kernel app
    PyObject *appModuleObj = PyImport_ImportModule("IPython.kernel.zmq.kernelapp");
    PyMapping_SetItemString(_globals, "app", appModuleObj);
    Py_XDECREF(appModuleObj);
    _kernelAppObj = PyRun_StringFlags("app.IPKernelApp()", Py_eval_input, _globals, _globals, NULL);
    
    // initialize kernel app
    PyObject *initialize = PyObject_GetAttrString(_kernelAppObj, "initialize");
    PyObject *argv = PyList_New(4);
    // TODO the strings are probably never released
    PyList_SetItem(argv, 0, PyString_FromString("-f"));
    PyList_SetItem(argv, 1, PyString_FromString([connectionFilePath UTF8String]));
    PyList_SetItem(argv, 2, PyString_FromString("--profile-dir"));
    PyList_SetItem(argv, 3, PyString_FromString([_profileDir UTF8String]));
    tuple = PyTuple_New(1);
    PyTuple_SetItem(tuple, 0, argv);
    PyObject_CallObject(initialize, tuple);
    Py_XDECREF(argv);
    Py_XDECREF(tuple);
    Py_XDECREF(initialize);
    
    // finally, start kernel app
    NSLog(@"starting kernel application");
    PyObject_CallMethod(_kernelAppObj, "start", NULL);
    
    if (completion)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            completion(notebook);
        });
    }
}

+ (void)closeNotebook:(Notebook *)notebook completion:(dispatch_block_t)completion
{
    if (notebook == nil)
    {
        return;
    }
    [_openNotebooks removeObject:notebook];
    
    // shutdown kernel
    CFAbsoluteTime start = CFAbsoluteTimeGetCurrent();
    [notebook.kernel stop:^{
        NSLog(@"--> notebook stopped in %.2fs", CFAbsoluteTimeGetCurrent() - start);
        if (completion)
        {
            completion();
        }
    }];
}

+ (NSArray *)openNotebooks
{
    return _openNotebooks;
}

#pragma mark - Helpers

+ (NSString *)connectionFilePathForKernelId:(NSString *)kernelId
{
    NSString *securityDir = [_profileDir stringByAppendingPathComponent:@"security"];
    NSString *connectionFile = [NSString stringWithFormat:@"kernel-%@.json", kernelId];
    return [securityDir stringByAppendingPathComponent:connectionFile];
}

+ (NSDictionary *)connectionConfigurationFromFile:(NSString *)connectionFile
{
    NSData *configurationData = [NSData dataWithContentsOfFile:connectionFile];
    NSError *error = nil;
    NSDictionary *configurationDict = [NSJSONSerialization JSONObjectWithData:configurationData options:0 error:&error];
    if (error)
    {
        NSLog(@"%@", error);
    }
    return configurationDict;
}

@end
