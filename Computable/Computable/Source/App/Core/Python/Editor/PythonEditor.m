//
//  PythonEditor.m
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonEditor.h"
#import "../Runtime/PythonRuntime.h"
#import <Python.h>

static const CFTimeInterval kMinSearchInterval = 0.5; // at most every 0.5s

static PyObject *_moduleObj;
static PyObject *_moduleDictObj;
static PyObject *_classObj;
static CFAbsoluteTime _lastSearch;

@implementation PythonEditor

+ (void)completionsForSource:(NSString *)source
                      atLine:(NSUInteger)line
                      column:(NSInteger)column
                      result:(void (^)(NSArray *completions))result
{
    if (source == nil || line == NSNotFound || column == NSNotFound)
    {
        return;
    }
    if (CFAbsoluteTimeGetCurrent() - _lastSearch < kMinSearchInterval)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            result(nil);
        });
        return;
    }
    _lastSearch = CFAbsoluteTimeGetCurrent();
    
    PythonRuntime *runtime = [PythonRuntime instance];
    [runtime execute:^{
        NSMutableArray *completions = [NSMutableArray array];
        
        if (!_moduleObj)
        {
            _moduleObj = PyImport_ImportModule("jedi");
        }
        if (!_moduleDictObj)
        {
            _moduleDictObj = _moduleObj ? PyModule_GetDict(_moduleObj) : NULL;
        }
        if (!_classObj)
        {
            _classObj = _moduleDictObj ? PyDict_GetItemString(_moduleDictObj, "Script") : NULL;
        }
        if (_classObj)
        {
            PyObject *arguments = Py_BuildValue("siis", source.UTF8String, (int)(line + 1), (int)column, "''");
            PyObject *instance = PyObject_CallObject(_classObj, arguments);
            PyObject *completionsObj = instance ? PyObject_CallMethod(instance, "completions", NULL) : NULL;
            if (completionsObj)
            {
                PyObject *sequence = PySequence_Fast(completionsObj, NULL);
                int len = PySequence_Size(sequence);
                for (int i = 0; i < len; i++) {
                    PyObject *completionObj = PySequence_Fast_GET_ITEM(sequence, i);
                    
                    PyObject *typeStringObj = PyObject_GetAttrString(completionObj, "type");
                    char *typeString = typeStringObj ? PyString_AsString(typeStringObj) : NULL;
                    PyObject *nameObj = PyObject_GetAttrString(completionObj, "name");
                    char *nameString = nameObj ? PyString_AsString(nameObj) : NULL;
                    PyObject *completeObj = PyObject_GetAttrString(completionObj, "complete");
                    char *completeString = completeObj ? PyString_AsString(completeObj) : NULL;

                    PyObject *args = Py_BuildValue("()");
                    PyObject *keywords = PyDict_New();
                    PyDict_SetItemString(keywords, "raw", Py_False);
                    PyDict_SetItemString(keywords, "fast", Py_True);
                    PyObject *docStringMethod = PyObject_GetAttrString(completionObj, "docstring");
                    PyObject *docStringObj = docStringMethod ? PyObject_Call(docStringMethod, args, keywords) : NULL;
                    char *docString = docStringObj ? PyString_AsString(docStringObj) : NULL;
                    Py_XDECREF(docStringMethod);
                    Py_XDECREF(keywords);
                    Py_XDECREF(args);

                    // params?
                    
                    Completion *completion = [Completion new];
                    if (typeString)
                    {
                        completion.type = [NSString stringWithUTF8String:typeString];
                    }
                    if (nameString)
                    {
                        completion.name = [NSString stringWithUTF8String:nameString];
                    }
                    if (completeString)
                    {
                        completion.completion = [NSString stringWithUTF8String:completeString];
                    }
                    if (docString && strlen(docString) > 0)
                    {
                        completion.docString = [NSString stringWithUTF8String:docString];
                    }
                    [completions addObject:completion];
                        
                    Py_XDECREF(typeStringObj);
                    Py_XDECREF(nameObj);
                    Py_XDECREF(completeObj);
                    Py_XDECREF(docStringObj);
                    /* DON'T DECREF item */
                }
                Py_XDECREF(sequence);
            }
            Py_XDECREF(completionsObj);
            Py_XDECREF(instance);
            Py_XDECREF(arguments);
        }
        
        _lastSearch = 0;
        if (result)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                result(completions);
            });
        }
    }];
}

@end
