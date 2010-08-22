#!/bin/sh

#FIXME: should be filled by CMake instead.
: ${ROBOTPKG_BASE=$HOME/devel/openrobots}

prefix=$ROBOTPKG_BASE
logdir="$prefix/var/log"

if ! test -d "$logdir"; then
    echo "$logdir does not exist"
    return 1
fi

echo "Removing logging information..."
rm -f $logdir/I*pgm
rm -f $logdir/Ires*ppm
rm -f $logdir/llvc-mbt.log
echo "Done."
