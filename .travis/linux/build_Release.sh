export QTDIR=/opt/qt510
export PATH=$QTDIR/bin:$PATH

git submodule update --init --recursive
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE -D WITH_TESTS=1 ..
make -j$(nproc)


echo "Copying AppDir..."
ls -R ../AppDir
cp -r ../AppDir ./

echo "Copying Steca binary into AppDir..."
ls -l ./main/Steca
ls -R ./AppDir
cp ./main/Steca ./AppDir/usr/bin/
ls -l ./AppDir/usr/bin/


echo "wgetting linuxdeployqt..."
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"

echo "chmod linuxdeployqt and setting env vars..."
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD) # linuxdeployqt uses this for naming the file

echo "Running linuxdeployqt..."
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/*.desktop -bundle-non-qt-libs
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/*.desktop -appimage
