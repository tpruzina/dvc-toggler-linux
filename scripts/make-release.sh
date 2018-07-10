#!/bin/bash

SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOTDIR="${SCRIPTDIR}/.."
DISTDIR="${ROOTDIR}/dist-build/root"
BUILDDIR="${ROOTDIR}/dist-build"
VERSION="0.0.5"
DEPENDS="libc6, qt5-default (>= 5.9.5), libxmu6, libxext6, libx11-6, libqt5svg5"

# build and install files into temporary release directory
meson $BUILDDIR
cd $BUILDDIR
ninja
DESTDIR=${DISTDIR} ninja install

# deb package
# 1) Create data.tar.xz (DISTDIR)
cd $DISTDIR
tar cfz data.tar.gz *
mv $DISTDIR/data.tar.gz $BUILDDIR
cd $BUILDDIR

# 2) Create control.tar.gz
mkdir $BUILDDIR/control
cd $BUILDDIR/control
CONTROL=$(cat << EOF
Package: dvc-toggler-linux
Source: dvc-toggler-linux (${VERSION}-$(git rev-parse --short HEAD))
Version: ${VERSION}
Architecture: amd64
Maintainer: Tomas Pruzina <pruzinat@gmail.com>
Depends: ${DEPENDS}
Priority: optional
Homepage: https://gitlab.com/pruzinat/dvc-toggler-linux
Description: DVC toggler
 A small Qt5 app that controls Digital Vibrance (a.k.a saturation) on per application basis.
 Inspired by windows DVC togglers.
EOF
)
echo "${CONTROL}" > control

MD5SUM=$(cd $DISTDIR; find . -type f | sed 's@\.\/@@' | xargs md5sum)
echo "${MD5SUM}" > md5sums

tar cvfz control.tar.gz *
mv control.tar.gz $BUILDDIR
cd $BUILDDIR

#3) Create debian-binary
echo 2.0 > debian-binary

# 4) Finally put it all together
ar r dvc-toggler-linux-$VERSION.deb debian-binary control.tar.gz data.tar.gz
