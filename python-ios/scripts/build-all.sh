#!/bin/bash

. $(dirname $0)/environment.sh

#try $(dirname $0)/build-libffi.sh
#try $(dirname $0)/build-python.sh
#try $(dirname $0)/reduce-python.sh
#try $(dirname $0)/build-pyzmq.sh
#try $(dirname $0)/build-numpy.sh
#try $(dirname $0)/build-f2c.sh
try $(dirname $0)/build-scipy.sh
#try $(dirname $0)/build-freetype.sh
#try $(dirname $0)/build-png.sh
#try $(dirname $0)/build-matplotlib.sh
#try $(dirname $0)/build-pandas.sh
#try $(dirname $0)/build-vtparse.sh
#try $(dirname $0)/build-bz2.sh
#try $(dirname $0)/build-faulthandler.sh

echo '== Build done'
echo "Available libraries in $BUILDROOT/lib"
echo $(ls $BUILDROOT/lib)
