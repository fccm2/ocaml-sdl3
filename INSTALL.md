You don't need pkg-config yet to compile these sources.  
 sudo apt-get install pkg-config

You can download, compile, and install the last stable release
of sdl3 with the following commands:  
```
wget https://github.com/libsdl-org/SDL/releases/download/release-3.2.6/SDL3-3.2.6.tar.gz
tar xzf SDL3-3.2.6.tar.gz
cd SDL-release-3.2.6/
cmake .
make
sudo make install
```

(You need about 160M of free space left on device to compile these
sources.)

