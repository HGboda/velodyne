#!/usr/bin/env bash

echo "##########################################################"
echo "#   ___________    ____ ______  __ __  _____             #"
echo "#   \____ \__  \ _/ ___\\____ \^|  ^|  \/  ___/          #"
echo "#   ^|  ^|_^> ^> __ \\  \___^|  ^|_^> ^>  ^|  /\___ \    #"
echo "#   ^|   __(____  /\___  ^>   __/^|____//____  ^>        #"
echo "#   ^|__^|       \/     \/^|__^|              \/         #"
echo "#                                                        #"
echo "##########################################################"
echo ""
echo "========================================"
echo "          PACPUS BUILD Linux            "
echo "========================================"


function usage {
    echo Usage:
    echo     build-linux.sh [install_dir]
    echo     [install-dir] is optional
    echo The script must be run in the build directory, which is 'trunk\build'
    echo ""
    echo Where:
    echo     install_dir   is the installation directory of the library
    echo ""
    echo For instance, here is how to build Pacpus:
    echo     ../scripts/build-linux.sh "/opt/Pacpus"
    echo ""
    exit 1
}

function build {
    # Initialize the build
    cmake .. -DPACPUS_INSTALL_DIR=$1 -DPACPUS_INSTALL_3RD=TRUE \
        -DPACPUS_USE_LOG=TRUE -DCMAKE_BUILD_TYPE=Release
    make
    sudo make install
    echo "Build finished"
    exit 0
}

#Check if the user is in the build directory
if [ `basename $PWD` == "build" ]; then
    if [ $# -ne 1 ]; then
        INST_PATH="/opt/pacpus"
    else
        INST_PATH=$1
    fi
    build $INST_PATH
else
    usage
fi