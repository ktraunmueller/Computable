# coding: utf-8

#-----------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------

# stdlib
import errno
import logging
import os
import random
import select
import signal
import socket
import sys
import threading
import time

import zmq
from zmq.eventloop import ioloop
#ioloop.install()

from zmq.eventloop.zmqstream import ZMQStream
from .kernel.kernelmanager import MappingKernelManager
from .notebook.nbmanager import NotebookManager
from .notebook.filenbmanager import FileNotebookManager

from IPython.config.application import catch_config_error, boolean_flag
from IPython.core.application import BaseIPythonApplication
from IPython.consoleapp import IPythonConsoleApp
from IPython.kernel import swallow_argv
from IPython.kernel.zmq.session import default_secure
from IPython.kernel.zmq.kernelapp import (
    kernel_flags,
    kernel_aliases,
)
from IPython.utils.importstring import import_item
from IPython.utils.localinterfaces import LOCALHOST
from IPython.utils import submodule
from IPython.utils.traitlets import (
    Dict, Unicode, Integer, List, Bool, Bytes,
    DottedObjectName
)
from IPython.utils import py3compat
from IPython.utils.path import filefind

#-----------------------------------------------------------------------------
# Aliases and Flags
#-----------------------------------------------------------------------------

flags = dict(kernel_flags)
notebook_flags = [ ]

aliases = dict(kernel_aliases)
aliases.update({
    'notebook-dir': 'NotebookManager.notebook_dir',
})

# remove ipkernel flags that are singletons, and don't make sense in
# multi-kernel evironment:
aliases.pop('f', None)

notebook_aliases = [u'notebook-dir', u'profile', u'profile-dir']

#-----------------------------------------------------------------------------
# NotebookApp
#-----------------------------------------------------------------------------

class NotebookApp(BaseIPythonApplication):

    name = 'ipython-notebook'
    
    classes = IPythonConsoleApp.classes + [MappingKernelManager, NotebookManager,
        FileNotebookManager]
    flags = Dict(flags)
    aliases = Dict(aliases)

    kernel_argv = List(Unicode)

    def _log_level_default(self):
        return logging.DEBUG

    # create requested profiles by default, if they don't exist:
    auto_create = Bool(True)

    notebook_manager_class = DottedObjectName('IPython.embedded.notebook.filenbmanager.FileNotebookManager',
        config=True,
        help='The notebook manager class to use.')

    def parse_command_line(self, argv=None):
        super(NotebookApp, self).parse_command_line(argv)
        
        if self.extra_args:
            f = os.path.abspath(self.extra_args[0])
            if os.path.isdir(f):
                nbdir = f
            else:
                self.file_to_run = f
                nbdir = os.path.dirname(f)
            self.config.NotebookManager.notebook_dir = nbdir

    def init_kernel_argv(self):
        """construct the kernel arguments"""
        # Scrub frontend-specific flags
        self.kernel_argv = swallow_argv(self.argv, notebook_aliases, notebook_flags)
        # Kernel should inherit default config file from frontend
        self.kernel_argv.append("--IPKernelApp.parent_appname='%s'" % self.name)
        # Kernel should get *absolute* path to profile directory
        self.kernel_argv.extend(["--profile-dir", self.profile_dir.location])

    def init_configurables(self):
        # force Session default to be secure
        default_secure(self.config)
        self.kernel_manager = MappingKernelManager(
            parent=self, log=self.log, kernel_argv=self.kernel_argv,
            connection_dir = self.profile_dir.security_dir,
        )
        kls = import_item(self.notebook_manager_class)
        self.notebook_manager = kls(parent=self, log=self.log)
        self.notebook_manager.load_notebook_names()
        #self.cluster_manager = ClusterManager(parent=self, log=self.log)
        #self.cluster_manager.update_profiles()

    @catch_config_error
    def initialize(self, argv=None):
        super(NotebookApp, self).initialize(argv)
        self.init_kernel_argv()
        self.init_configurables()

    def cleanup_kernels(self):
        """Shutdown all kernels.
        
        The kernels will shutdown themselves when this process no longer exists,
        but explicit shutdown allows the KernelManagers to cleanup the connection files.
        """
        self.log.info('Shutting down kernels')
        self.kernel_manager.shutdown_all()

    def start(self):
        """ Start the IPython Notebook server app, after initialization

        This method takes no arguments so all configuration and initialization
        must be done prior to calling this method."""
        try:
            ioloop.IOLoop.instance().start()
        except:
            print "Error on stop:", sys.exc_info()[0]
            raise
        finally:
            self.cleanup_kernels()

    def stop(self):
        try:
            ioloop.IOLoop.instance().stop()
        except:
            print "Error on stop:", sys.exc_info()[0]
            raise
        finally:
            self.cleanup_kernels()

#-----------------------------------------------------------------------------
# Main entry point
#-----------------------------------------------------------------------------

#launch_new_instance = NotebookApp.launch_instance

