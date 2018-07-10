#!bin/bash

# use alien tool to convert deb to rpm
RPM=$(alien -k --to-rpm --fixperms $ROOTDIR/releases/dvc-toggler-linux-$VERSION-$ARCH.deb 2>&1 | tail -n 1 | awk '{print $1}')
mv $RPM dvc-toggler-linux-$VERSION-$ARCH.rpm
cp -v dvc-toggler-linux-$VERSION-$ARCH.rpm $ROOTDIR/releases/
