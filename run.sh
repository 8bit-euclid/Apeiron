WORKDIR=$PWD
BUILD=~/Dropbox/Projects/Apeiron/build
rm -f Apeiron
cd $BUILD
make -j 16 Apeiron
cp $BUILD/bin/Apeiron $WORKDIR/.
cd $WORKDIR
./Apeiron
