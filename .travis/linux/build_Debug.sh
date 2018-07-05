export QTDIR=/opt/qt510
export PATH=$QTDIR/bin:$PATH

git submodule update --init --recursive
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE -D WITH_TESTS=1 ..
make -j$(nproc)


#cp -r ../AppDir ./
#cp ./main/Steca ./AppDir/usr/bin

#wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
#chmod a+x linuxdeployqt-continuous-x86_64.AppImage
#unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
#export VERSION=$(git rev-parse --short HEAD) # linuxdeployqt uses this for naming the file
#./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/*.desktop -bundle-non-qt-libs
#./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/*.desktop -appimage
