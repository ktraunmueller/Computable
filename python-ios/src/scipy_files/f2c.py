from __future__ import division, absolute_import, print_function

import re
import os
import sys
import tempfile
from subprocess import Popen, PIPE, STDOUT

from distutils.ccompiler import \
    CCompiler, gen_preprocess_options, gen_lib_options
from distutils.errors import \
    DistutilsExecError, CompileError, LibError, LinkError

from numpy.distutils.fcompiler import FCompiler

compilers = ['F2CFCompiler']

TARGET_R = re.compile("Target: ([a-zA-Z0-9_\-]*)")

class F2CFCompiler(FCompiler):
    compiler_type = 'f2c'
    compiler_aliases = ('f2c',)
    description = 'f2c Fortran to C converter'
    
    def version_match(self, version_string):
        return '1.0'
    
    # set up linker_so to use the ld in the environment (from XcodeDefault.xctoolchain/usr/bin/)
    ld = os.environ.get('LD', None)
    ar = os.environ.get('AR', None)
    cc = os.environ.get('CC', None)
    
    possible_executables = ['g77']
    executables = {
        'version_cmd'  : [None, "--version"],
        'compiler_f77' : [None],
        'compiler_f90' : None, # Use --fcompiler=gnu95 for f90 codes
        'compiler_fix' : None,
        'linker_so'    : [cc, "-c"],
        'archiver'     : [ar],
        'ranlib'       : [None],
        'linker_exe'   : [ld]
    }
    module_dir_switch = None
    module_include_switch = None
    
    #pic_flags = ['-fPIC']
    pic_flags = []
    g2c = ''
    
    suggested_f90_compiler = 'gnu95'
    
    def get_flags_linker_so(self):
        opt = self.linker_so[1:]
        if sys.platform=='darwin':
            target = os.environ.get('MACOSX_DEPLOYMENT_TARGET', None)
            # If MACOSX_DEPLOYMENT_TARGET is set, we simply trust the value
            # and leave it alone.  But, distutils will complain if the
            # environment's value is different from the one in the Python
            # Makefile used to build Python.  We let disutils handle this
            # error checking.
            if not target:
                # If MACOSX_DEPLOYMENT_TARGET is not set in the environment,
                # we try to get it first from the Python Makefile and then we
                # fall back to setting it to 10.3 to maximize the set of
                # versions we can work with.  This is a reasonable default
                # even when using the official Python dist and those derived
                # from it.
                import distutils.sysconfig as sc
                g = {}
                filename = sc.get_makefile_filename()
                sc.parse_makefile(filename, g)
                target = g.get('MACOSX_DEPLOYMENT_TARGET', '10.3')
                os.environ['MACOSX_DEPLOYMENT_TARGET'] = target
            
            opt.extend(['-undefined', 'dynamic_lookup', '-dynamic'])
        return opt
    
    def get_libgcc_dir(self):
        return None
    
    def get_library_dirs(self):
        buildroot = os.environ.get('BUILDROOT', None)
        opt = [ os.path.join(buildroot, 'lib') ]
        return opt
    
    def get_libraries(self):
        opt = [ 'python2.7', 'ffi', 'sqlite3', 'z', 'f2c' ]
        return opt
    
    def get_flags_debug(self):
        return []
    
    def get_flags_opt(self):
        return []
    
    def _c_arch_flags(self):
        """ Return detected arch flags from CFLAGS """
        from distutils import sysconfig
        try:
            cflags = sysconfig.get_config_vars()['CFLAGS']
        except KeyError:
            return []
        arch_re = re.compile(r"-arch\s+(\w+)")
        arch_flags = []
        for arch in arch_re.findall(cflags):
            arch_flags += ['-arch', arch]
        return arch_flags
    
    def get_flags_arch(self):
        return []
    
    # copied from distutils.CCompiler
    def _compile(self, obj, src, ext, cc_args, extra_postargs, pp_opts):
        cwd = os.getcwd()
        oneup = os.path.split(cwd)[0]
        
        # step 1: translate .f to .c
        dir = os.path.dirname(src)
        file = os.path.split(src)[1]
        
        name = os.path.splitext(src)[0]
        ext = os.path.splitext(src)[1]
        cfile  = name + '.c'
        
        if (not os.path.isfile(cfile)):
            # only create .c if not exists (don't overwrite patched files)
            f_compiler = self.compiler_f77[0]
            os.chdir(dir)
            try:
                self.spawn([ f_compiler ] + [ file ])
            except DistutilsExecError, msg:
                raise CompileError, msg
            os.chdir(cwd)
        
        # step 2: compile .c using C compiler
        c_sources = [ cfile ]
        
        output_dir = obj[0:len(obj) - len(cfile)] # os.path.split(obj)[0]
        macros = None
        include_dirs = [ os.path.join(oneup, 'f2c', 'libf2c') ]
        try:
            self.c_compiler.compile(c_sources, output_dir, macros, include_dirs)
        except DistutilsExecError, msg:
            raise CompileError, msg

def _can_target(cmd, arch):
    """Return true is the command supports the -arch flag for the given
        architecture."""
    newcmd = cmd[:]
    fid, filename = tempfile.mkstemp(suffix=".f")
    try:
        d = os.path.dirname(filename)
        output = os.path.splitext(filename)[0] + ".o"
        try:
            newcmd.extend(["-arch", arch, "-c", filename])
            p = Popen(newcmd, stderr=STDOUT, stdout=PIPE, cwd=d)
            p.communicate()
            return p.returncode == 0
        finally:
            if os.path.exists(output):
                os.remove(output)
    finally:
        os.remove(filename)
    return False

if __name__ == '__main__':
    from distutils import log
    log.set_verbosity(2)
    
    compiler = F2CFCompiler()
    compiler.customize()
    print(compiler.get_version())