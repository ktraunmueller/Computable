//
//  PythonRuntime.m
//  PyBooks-Common
//
//  Created by Karl Traunmüller on 17.01.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "PythonRuntime.h"
#import "../../../../Common/Utilities/Filesystem.h"

static NSString *const PythonLibFolderName = @"python-lib";

// pyzmq module init functions
extern PyMODINIT_FUNC init_device();
extern PyMODINIT_FUNC init_poll();
extern PyMODINIT_FUNC init_version();
extern PyMODINIT_FUNC initconstants();
extern PyMODINIT_FUNC initcontext();
extern PyMODINIT_FUNC initerror();
extern PyMODINIT_FUNC initmessage();
extern PyMODINIT_FUNC initsocket();
extern PyMODINIT_FUNC initutils();
extern PyMODINIT_FUNC initmonitoredqueue();
extern PyMODINIT_FUNC initinitthreads();
extern PyMODINIT_FUNC initrebuffer();

// numpy module init functions
extern PyMODINIT_FUNC init_dotblas();
extern PyMODINIT_FUNC initmultiarray();
extern PyMODINIT_FUNC initscalarmath();
extern PyMODINIT_FUNC initumath();
extern PyMODINIT_FUNC initfftpack_lite();
extern PyMODINIT_FUNC init_compiled_base();
extern PyMODINIT_FUNC init_umath_linalg();
extern PyMODINIT_FUNC initlapack_lite();
extern PyMODINIT_FUNC init_capi();
extern PyMODINIT_FUNC initmtrand();

// scipy module init functions
extern PyMODINIT_FUNC init__odrpack();
extern PyMODINIT_FUNC init_arpack();
extern PyMODINIT_FUNC init_cobyla();
extern PyMODINIT_FUNC init_dop();
extern PyMODINIT_FUNC init_fblas();
extern PyMODINIT_FUNC init_flapack();
extern PyMODINIT_FUNC init_fftpack();
extern PyMODINIT_FUNC init_fitpack();
extern PyMODINIT_FUNC init_flinalg();
extern PyMODINIT_FUNC init_interpolate();
extern PyMODINIT_FUNC init_interpolative();
extern PyMODINIT_FUNC init_iterative();
extern PyMODINIT_FUNC init_lbfgsb();
extern PyMODINIT_FUNC init_min_spanning_tree();
extern PyMODINIT_FUNC init_minpack();
extern PyMODINIT_FUNC init_ni_label();
extern PyMODINIT_FUNC init_nnls();
extern PyMODINIT_FUNC init_odepack();
extern PyMODINIT_FUNC init_quadpack();
extern PyMODINIT_FUNC init_rank();
extern PyMODINIT_FUNC init_shortest_path();
extern PyMODINIT_FUNC init_slsqp();
extern PyMODINIT_FUNC init_spectral();
extern PyMODINIT_FUNC init_superlu();
extern PyMODINIT_FUNC init_tools();
extern PyMODINIT_FUNC init_traversal();
extern PyMODINIT_FUNC init_ufuncs();
extern PyMODINIT_FUNC init_ufuncs_cxx();
extern PyMODINIT_FUNC init_bsr();
extern PyMODINIT_FUNC initcalc_lwork();
extern PyMODINIT_FUNC initcblas();
extern PyMODINIT_FUNC initckdtree();
extern PyMODINIT_FUNC initclapack();
extern PyMODINIT_FUNC init_convolution_kernel();
extern PyMODINIT_FUNC initconvolve();
extern PyMODINIT_FUNC init_coo();
extern PyMODINIT_FUNC init_csc();
extern PyMODINIT_FUNC init_csgraph();
extern PyMODINIT_FUNC init_csr();
extern PyMODINIT_FUNC initdfitpack();
extern PyMODINIT_FUNC init_dia();
extern PyMODINIT_FUNC init_distance_wrap();
//extern PyMODINIT_FUNC initfblas();
//extern PyMODINIT_FUNC initflapack();
extern PyMODINIT_FUNC initfutil();
extern PyMODINIT_FUNC init_hierarchy_wrap();
extern PyMODINIT_FUNC initinterpnd();
extern PyMODINIT_FUNC initlsoda();
extern PyMODINIT_FUNC initminpack2();
extern PyMODINIT_FUNC initmio5_utils();
extern PyMODINIT_FUNC initmio_utils();
extern PyMODINIT_FUNC initmoduleTNC();
extern PyMODINIT_FUNC init_nd_image();
extern PyMODINIT_FUNC initqhull();
extern PyMODINIT_FUNC initsigtools();
extern PyMODINIT_FUNC initspecfun();
extern PyMODINIT_FUNC initspline();
extern PyMODINIT_FUNC initstatlib();
//extern PyMODINIT_FUNC initmvn();
extern PyMODINIT_FUNC initstreams();
extern PyMODINIT_FUNC initvode();
extern PyMODINIT_FUNC initvonmises_cython();
extern PyMODINIT_FUNC init_vq();
extern PyMODINIT_FUNC init_zeros();
extern PyMODINIT_FUNC initcblas();

// matplotlib module init functions
extern PyMODINIT_FUNC init_backend_agg();
extern PyMODINIT_FUNC init_delaunay();
extern PyMODINIT_FUNC init_tri();
extern PyMODINIT_FUNC init_image();
extern PyMODINIT_FUNC init_path();
extern PyMODINIT_FUNC init_png();
extern PyMODINIT_FUNC init_cntr();
extern PyMODINIT_FUNC initft2font();
extern PyMODINIT_FUNC initnxutils();

// pandas module init functions
extern void init_numpy_helper();    // calls import_array() for module numpy_helper.c
extern PyMODINIT_FUNC initalgos();
extern PyMODINIT_FUNC inithashtable();
extern PyMODINIT_FUNC initindex();
extern PyMODINIT_FUNC initlib();
extern PyMODINIT_FUNC initpdparser();
extern PyMODINIT_FUNC init_sparse();
extern PyMODINIT_FUNC init_testing();
extern PyMODINIT_FUNC initObjToJSON();
extern PyMODINIT_FUNC initjson();
extern PyMODINIT_FUNC inittslib();
extern PyMODINIT_FUNC initmsgpack();

// faulthandler module init function
extern PyMODINIT_FUNC initfaulthandler();

@interface RuntimeThread : NSObject

@property (nonatomic, assign) PyThreadState *mainThreadState;
@property (nonatomic, copy) dispatch_block_t block;

@end

@implementation RuntimeThread
@end

@implementation PythonRuntime
{
    PyObject *_globals;
    PyObject *_numpyCApi;
    PyThreadState *_mainThreadState;
    dispatch_queue_t _queue;
}

#pragma mark - Statics

+ (NSString *)runtimePath
{
    return [[Filesystem scopedApplicationSupportDirectory] stringByAppendingPathComponent:PythonLibFolderName];
}

+ (PythonRuntime *)instance
{
    static PythonRuntime *_instance = nil;
    if (_instance == nil)
    {
        _instance = [[PythonRuntime alloc] init];
    }
    return _instance;
}

+ (void)print:(char *)prefix object:(PyObject *)obj
{
    if (prefix)
    {
        printf("%s: ", prefix);
    }
    PyObject_Print(obj, stdout, 0);
    printf("\n");
}

#pragma mark - PythonRuntime

- (id)init
{
    if (self = [super init])
    {
        _queue = dispatch_queue_create("Python runtime", DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

- (void)dealloc
{
    [self shutdown:nil];
}

- (BOOL)needsRuntimeFolderSetup
{
    return ![[NSFileManager defaultManager] fileExistsAtPath:[PythonRuntime runtimePath]];
}

- (void)setupRuntimeFolder:(dispatch_block_t)completion
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *runtimePath = [PythonRuntime runtimePath];
    if ([fileManager fileExistsAtPath:runtimePath])
    {
        if (completion)
        {
            completion();
        }
        return;
    }
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        NSString *runtimeBasePath = [runtimePath stringByDeletingLastPathComponent];
        NSError *error = nil;
        [fileManager createDirectoryAtPath:runtimeBasePath
               withIntermediateDirectories:YES
                                attributes:nil
                                     error:&error];
        [Filesystem addSkipBackupAttributeToItemAtPath:runtimeBasePath];
        if (error)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"%@", error);
            });
        }
        
        NSString *resourcesPath = [[NSBundle mainBundle] resourcePath];
        NSString *bundledRuntimePath = [resourcesPath stringByAppendingPathComponent:PythonLibFolderName];
        [fileManager copyItemAtPath:bundledRuntimePath
                             toPath:runtimePath
                              error:&error];
        if (error)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"%@", error);
            });
        }
        
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion();
            });
        }
    });
}

- (void)launch:(dispatch_block_t)completion
{
    dispatch_sync(_queue, ^{
        char *sys_path = (char *)[[PythonRuntime runtimePath] UTF8String];
        Py_SetPythonHome(sys_path);
        Py_NoSiteFlag = 1;
        
        // register zmq module init functions
        PyImport_AppendInittab("zmq.utils", initutils);
        PyImport_AppendInittab("zmq.utils.initthreads", initinitthreads);
        PyImport_AppendInittab("zmq.utils.rebuffer", initrebuffer);
        PyImport_AppendInittab("zmq.backend.cython.constants", initconstants);
        PyImport_AppendInittab("zmq.backend.cython._device", init_device);
        PyImport_AppendInittab("zmq.backend.cython._poll", init_poll);
        PyImport_AppendInittab("zmq.backend.cython._version", init_version);
        PyImport_AppendInittab("zmq.backend.cython.context", initcontext);
        PyImport_AppendInittab("zmq.backend.cython.error", initerror);
        PyImport_AppendInittab("zmq.backend.cython.message", initmessage);
        PyImport_AppendInittab("zmq.backend.cython.socket", initsocket);
        PyImport_AppendInittab("zmq.devices.monitoredqueue", initmonitoredqueue);
        
        // register numpy module init functions
        PyImport_AppendInittab("numpy.core._dotblas", init_dotblas);
        PyImport_AppendInittab("numpy.core.multiarray", initmultiarray);
        PyImport_AppendInittab("numpy.core.scalarmath", initscalarmath);
        PyImport_AppendInittab("numpy.core.umath", initumath);
        PyImport_AppendInittab("numpy.fft.fftpack_lite", initfftpack_lite);
        PyImport_AppendInittab("numpy.lib._compiled_base", init_compiled_base);
        PyImport_AppendInittab("numpy.linalg._umath_linalg", init_umath_linalg);
        PyImport_AppendInittab("numpy.linalg.lapack_lite", initlapack_lite);
        PyImport_AppendInittab("numpy.numarray._capi", init_capi);
        PyImport_AppendInittab("numpy.random.mtrand", initmtrand);
        
        // register scipy module init funcitons
        PyImport_AppendInittab("scipy.cluster._hierarchy_wrap", init_hierarchy_wrap);
        PyImport_AppendInittab("scipy.cluster._vq", init_vq);
        PyImport_AppendInittab("scipy.fftpack._fftpack", init_fftpack);
        PyImport_AppendInittab("scipy.fftpack.convolve", initconvolve);
        PyImport_AppendInittab("scipy.integrate._dop", init_dop);
        PyImport_AppendInittab("scipy.integrate._odepack", init_odepack);
        PyImport_AppendInittab("scipy.integrate._quadpack", init_quadpack);
        PyImport_AppendInittab("scipy.integrate.lsoda", initlsoda);
        PyImport_AppendInittab("scipy.integrate.vode", initvode);
        PyImport_AppendInittab("scipy.interpolate._fitpack", init_fitpack);
        PyImport_AppendInittab("scipy.interpolate._interpolate", init_interpolate);
        PyImport_AppendInittab("scipy.interpolate.dfitpack", initdfitpack);
        PyImport_AppendInittab("scipy.interpolate.interpnd", initinterpnd);
        PyImport_AppendInittab("scipy.io.matlab.mio_utils", initmio_utils);
        PyImport_AppendInittab("scipy.io.matlab.mio5_utils", initmio5_utils);
        PyImport_AppendInittab("scipy.io.matlab.streams", initstreams);
        PyImport_AppendInittab("scipy.linalg._fblas", init_fblas);
        PyImport_AppendInittab("scipy.linalg._flapack", init_flapack);
        PyImport_AppendInittab("scipy.linalg._flinalg", init_flinalg);
        PyImport_AppendInittab("scipy.linalg._interpolative", init_interpolative);
        PyImport_AppendInittab("scipy.linalg.calc_lwork", initcalc_lwork);
        PyImport_AppendInittab("scipy.ndimage._nd_image", init_nd_image);
        PyImport_AppendInittab("scipy.ndimage._ni_label", init_ni_label);
        PyImport_AppendInittab("scipy.odr.__odrpack", init__odrpack);
        PyImport_AppendInittab("scipy.optimize._cobyla", init_cobyla);
        PyImport_AppendInittab("scipy.optimize._lbfgsb", init_lbfgsb);
        PyImport_AppendInittab("scipy.optimize._minpack", init_minpack);
        PyImport_AppendInittab("scipy.optimize._nnls", init_nnls);
        PyImport_AppendInittab("scipy.optimize._slsqp", init_slsqp);
        PyImport_AppendInittab("scipy.optimize._zeros", init_zeros);
        PyImport_AppendInittab("scipy.optimize.minpack2", initminpack2);
        PyImport_AppendInittab("scipy.optimize.moduleTNC", initmoduleTNC);
        PyImport_AppendInittab("scipy.signal._spectral", init_spectral);
        PyImport_AppendInittab("scipy.signla.sigtools", initsigtools);
        PyImport_AppendInittab("scipy.signal.spline", initspline);
        PyImport_AppendInittab("scipy.sparse.csgraph._min_spanning_tree", init_min_spanning_tree);
        PyImport_AppendInittab("scipy.sparse.csgraph._shortest_path", init_shortest_path);
        PyImport_AppendInittab("scipy.sparse.csgraph._tools", init_tools);
        PyImport_AppendInittab("scipy.sparse.csgraph._traversal", init_traversal);
        PyImport_AppendInittab("scipy.sparse.linalg.dsolve._superlu", init_superlu);
        PyImport_AppendInittab("scipy.sparse.linalg.eigen.arpack._arpack", init_arpack);
        PyImport_AppendInittab("scipy.sparse.linalg.isolve._iterative", init_iterative);
        PyImport_AppendInittab("scipy.sparse.sparsetools._bsr", init_bsr);
        PyImport_AppendInittab("scipy.sparse.sparsetools._coo", init_coo);
        PyImport_AppendInittab("scipy.sparse.sparsetools._csc", init_csc);
        PyImport_AppendInittab("scipy.sparse.sparsetools._csgraph", init_csgraph);
        PyImport_AppendInittab("scipy.sparse.sparsetools._csr", init_csr);
        PyImport_AppendInittab("scipy.sparse.sparsetools._dia", init_dia);
        PyImport_AppendInittab("scipy.spatial._distance_wrap", init_distance_wrap);
        PyImport_AppendInittab("scipy.spatial.ckdtree", initckdtree);
        PyImport_AppendInittab("scipy.spatial.qhull", initqhull);
        PyImport_AppendInittab("scipy.special._ufuncs", init_ufuncs);
        PyImport_AppendInittab("scipy.special._ufuncs_cxx", init_ufuncs_cxx);
        PyImport_AppendInittab("scipy.special.specfun", initspecfun);
        PyImport_AppendInittab("scipy.stats._rank", init_rank);
        PyImport_AppendInittab("scipy.stats.futil", initfutil);
        //    PyImport_AppendInittab("scipy.stats.mvn", initmvn); // not F77 compatible
        PyImport_AppendInittab("scipy.stats.statlib", initstatlib);
        PyImport_AppendInittab("scipy.stats.vonmises_cython", initvonmises_cython);
        
        // register matplotlib module init functions
        PyImport_AppendInittab("matplotlib._cntr", init_cntr);
        PyImport_AppendInittab("matplotlib._delaunay", init_delaunay);
        PyImport_AppendInittab("matplotlib._image", init_image);
        PyImport_AppendInittab("matplotlib._path", init_path);
        PyImport_AppendInittab("matplotlib._png", init_png);
        PyImport_AppendInittab("matplotlib._tri", init_tri);
        PyImport_AppendInittab("matplotlib.backends._backend_agg", init_backend_agg);
        PyImport_AppendInittab("matplotlib.ft2font", initft2font);
        PyImport_AppendInittab("nxutils", initnxutils);
        
        // register pandas module init functions
        PyImport_AppendInittab("pandas.algos", initalgos);
        PyImport_AppendInittab("pandas.index", initindex);
        PyImport_AppendInittab("pandas.core.sparse", init_sparse);
        PyImport_AppendInittab("pandas.hashtable", inithashtable);
        PyImport_AppendInittab("pandas.json", initjson);
        PyImport_AppendInittab("pandas.lib", initlib);
        PyImport_AppendInittab("pandas.pdparser", initpdparser);
        PyImport_AppendInittab("pandas.tslib", inittslib);
        PyImport_AppendInittab("pandas.msgpack", initmsgpack);
        PyImport_AppendInittab("pandas.testing", init_testing);
        //        PyImport_AppendInittab("pandas.ObjToJSON", initObjToJSON); // no module?
        
        // register faulthandler module init functions
        //        PyImport_AppendInittab("faulthandler", initfaulthandler);
        
        NSLog(@"Starting Python runtime...");
        
        Py_Initialize();
        PyEval_InitThreads();
        PySys_SetPath(sys_path);
        
        NSString *bootScript =
        @"import platform\n"
        @"print 'This is Python', platform.python_version()";
        PyRun_SimpleString((char *)[bootScript UTF8String]);
        
        // TODO add to notebook preamble
        //        NSString *enableFaulthandler =
        //        @"import faulthandler\n"
        //        @"faulthandler.enable()";
        //        PyRun_SimpleString((char *)[enableFaulthandler UTF8String]);
        
        // populate numpy finfo cache while nobody holds the GIL
        // (the arithmentic underflow exception handler needs the GIL, which would lock up once we've started up our own python code)
        // see numerictypes.py for valid types
        NSString *finfo =
        @"import numpy\n"
        @"numpy.finfo(float).eps\n";
        PyRun_SimpleString((char *)[finfo cStringUsingEncoding:NSUTF8StringEncoding]);
        
        // this is part of pandas (calling import_array)
        init_numpy_helper();
        
//        PyRun_SimpleString("import jedi");  // check for import errors
        
        _globals = PyDict_New();
        _mainThreadState = PyEval_SaveThread();
        PyThreadState_Swap(_mainThreadState);
        
        if (completion)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion();
            });
        }
    });
}

- (void)shutdown:(dispatch_block_t)completion
{
    dispatch_async(_queue, ^{
        NSLog(@"Stopping Python runtime...");
        _mainThreadState = PyEval_SaveThread();
        Py_Finalize();
        NSLog(@"Python runtime stopped");
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (completion)
            {
                completion();
            }
        });
    });
}

- (PyObject *)globals
{
    return _globals;
}

- (void)execute:(dispatch_block_t)block
{
    [self execute:block completion:nil];
}

- (void)execute:(dispatch_block_t)block completion:(dispatch_block_t)completion
{
    dispatch_async(_queue, ^{
        PyGILState_STATE gstate = PyGILState_Ensure();
        PyThreadState *threadState = PyThreadState_New(_mainThreadState->interp);
        PyThreadState *state = PyThreadState_Swap(threadState);
        
        block();
        
        PyThreadState_Swap(state);
        PyThreadState_Delete(threadState);
        PyGILState_Release(gstate);
        
        if (completion)
        {
            completion();
        }
    });
}

- (void)detachThread:(dispatch_block_t)block
{
    RuntimeThread *thread = [[RuntimeThread alloc] init];
    thread.block = block;
    thread.mainThreadState = _mainThreadState;
    
    [NSThread detachNewThreadSelector:@selector(main:) toTarget:self withObject:thread];
}

- (void)main:(RuntimeThread *)thread
{
    @autoreleasepool
    {
        PyGILState_STATE gstate = PyGILState_Ensure();
        PyThreadState *threadState = PyThreadState_New(thread.mainThreadState->interp);
        PyThreadState *state = PyThreadState_Swap(threadState);
        
        thread.block();
        
        PyThreadState_Swap(state);
        PyThreadState_Delete(threadState);
        PyGILState_Release(gstate);
    }
}

@end
