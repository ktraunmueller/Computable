from IPython.embedded.notebookapp import *

app = NotebookApp()
app.initialize()
app.kernel_manager.start_kernel()
app.start()