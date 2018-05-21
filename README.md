# CNC Manual Controller

**CNC Manual Controller** (*CNC-MC*) is an open-source solution for controlling CNC machines, for Windows, Mac OS X and Linux.

Its core is developed in C++, based on the [Qt framework](https://www.qt.io/ "Qt framework").


## Quick Overview

This programme was initially designed for controlling a CNC machine driven by a *Velleman VM110N* experimentation card through an USB port.

The Velleman VM110N library for Windows can be found in subdirectory `./src/3rd/velleman/bin/k8055d.dll`. The libk8055 library is used for Linux and MacOSX and can be found in subdirectory `./src/3rd/libk8055/`. 


## 3rd-Party Libraries

### Simple DirectMedia Layer

	SDL v1.2.15

The Simple DirectMedia Layer (SDL for short) is a cross-platfrom library
designed to make it easy to write multi-media software, such as games and
emulators.

The Simple DirectMedia Layer library source code is available from:
[http://www.libsdl.org/](http://www.libsdl.org/ "SDL")

This library is distributed under the terms of the GNU LGPL license:
[http://www.gnu.org/copyleft/lesser.html](http://www.gnu.org/copyleft/lesser.html "GPL")


### K8055D Library

	K8055D v5.0.0.0

Dynamic Link Library for the K8055N / VM110N interface board.

Public domain license

[http://www.velleman.be/](http://www.velleman.be/ "Velleman")


## Installation

1. Download the most recent package
[here](https://github.com/setvisible/cnc-manual-controller/releases "Last Releases").

2. Unzip:

        $ tar zxvf cnc-manual-controller.tar.gz
        $ cd cnc-manual-controller/

3. Build

     - On Linux or Mac OS X

            $ mkdir -p build
            $ cd build/
            $ cmake ..              # Perform a system check.

            Then
            $ make -j               # Build the program(s).
            $ make strip            # Remove unneeded symbols.

            Finally
            $ sudo make install     # Install the program(s).

        Enjoy *CNC-MC* using `CNCManualController`.

        Rem: On MacOSX, you may have to type
        `$ chmod +x install.sh`
        after `$ make` for installing.


     - On Windows (with MinGW)

            > mkdir build
            > cd build/
            > cmake .. -G "MinGW Makefiles"

            Then
            > make -j
            > make install/strip

            Finally (could require admin rights)
            > make install



     - On Windows (with MSVC 2015)

            > mkdir build
            > cd build/
            > cmake .. -G "Visual Studio 14 2015"

            > start .
            Then, double click the Visual Studio project (vcxproj).

        Enjoy *CNC-MC* starting `CNCManualController.exe`.


## License

The code is released under the [MIT License](LICENSE "LICENSE").
