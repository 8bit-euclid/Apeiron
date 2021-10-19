WORKDIR=$PWD
BUILD=~/Dropbox/Projects/Apeiron/build
cd $BUILD
make -j 8 Apeiron
cp $BUILD/bin/Apeiron $WORKDIR/.
cd $WORKDIR
./Apeiron
