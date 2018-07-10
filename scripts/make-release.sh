#!/bin/bash

export SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export ROOTDIR="${SCRIPTDIR}/.."
export DISTDIR="${ROOTDIR}/dist-build/root"
export BUILDDIR="${ROOTDIR}/dist-build"

export ARCH="amd64"
export VERSION="0.0.5"

if [ ! -d "$BUILDDIR" ]; then
    meson $BUILDDIR
fi

cd $BUILDDIR
ninja
DESTDIR=$DISTDIR ninja install

. $SCRIPTDIR/make-deb-package.sh
. $SCRIPTDIR/deb-to-rpm.sh
