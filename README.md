Content
==================

 * Desura
 * Build Desura on Linux
 * Build Desura on Windows
 * Special notes for advanced users
 * Closing Remarks

Desura
===================

Desura is a gaming client that allows users to one click download and install games and game modification.
For more information, visit http://github.com/lindenlab/desura-app/wiki/

This Desura project is the open source from which the commercial
Desura product client is built. It can also be built as "Desurium" for
private use and experimentation with new features and fixes. For
information on the licensing of the project, see the LICENSE.md file.

Build Desura on Linux
===================

Note: Due to not being able to use all commits from the GPL3 version, linux builds are not currently working.

Dependencies
------------

You can install dependencies by package manager or install_deps.sh

Required dependencies:

 * GCC (4.8 or later) or Clang (3.4 or later) - compiler multilib support for 32 bit compatibility on 64 bit systems
 * cmake (2.8.5 or later)
 * Freetype
 * GLib2
 * GTK2
 * libEvent
 * libNotify (0.7 or later)
 * libX11 (with libXt)
 * Patch
 * Python

Optional dependencies (will be build by cmake if not found):

 * Boost (filesystem, system)
 * BZip2
 * Curl (7.19.1 or later)
 * Sqlite3
 * tinyxml2
 * wxWidgets (3.0.0 or later)

Optional dependencies (only in special cases):

 * Gtest (only when "WITH_GTEST" switch set to "ON")
 * OpenSSL (only for building Curl)
 * PkgConfig (for building DEB and RPM packages)

Third party bundled libraries:

 * Breakpad
 * CEF (Chromium Embedded)
 * V8

Installation
------------

Open Terminal in Desurium source directory and run:

    ./build_desura.sh

or you can speed up things by running:

    ./build_desura.sh -j `getconf _NPROCESSORS_ONLN`

or you can build DEB package:

    ./build_desura.sh pack DEB

or you can build RPM package:

    ./build_desura.sh pack RPM

Finally run Desurium using:

    ./install/desura

or install created DEB or RPM package

For informations on advanced arguments of the build script, run:

    ./build_desura.sh help

Build Desura on Windows
=====================

Download and install:

 * Svn http://www.sliksvn.com/en/download or http://tortoisesvn.net/downloads.html
 * Msvc 2013 or higher (as C++11 features are in use)
 * Python 2.7 x86 http://www.python.org/getit/
 * CMake 2.8 http://www.cmake.org/
 * Patch either from msysGit or git bash
 * (Optional) Windows DirectX 9 SDK for the CEF build https://www.microsoft.com/en-us/download/details.aspx?id=6812

Note: Make sure you add svn and python to your environment PATH

## Command line build:

Open a 32bit cmd.exe with env vars set up by vc or other scripts (so we have a full build environment) and run:

    ./build_desura.bat

wait


## Visual studio build:

Open a 32bit cmd.exe with env vars set up by vc or other scripts (so we have a full build environment) and run:

    ./build_desura_vis.bat
	
* Set BUILD_ALL as startup project
* In properties for BUILD_ALL set debugging options:
 * Command to source code dir + "\build_vis\Debug_Out\desura.exe"
 * Working Directory to source code dir + "\build_vis\Debug_Out"
* Build ThirdParty folder first (race condition some times if you dont)
* Build BUILD_ALL
 
Should be able to run and debug Desura now.

Special notes for advanced users
=====================

Desura is using the cmake build system for configuration on prject files for several build systems (make, VS, nmake, ...).
At the top of CMakeFile.txt is a documented list of options

a "normal" way to configure cmake would be something like this:

  * UNIX-based systems:
    1. mkdir build
    2. cd build
    3. cmake ..
    4. make
    5. sudo make install (optionally)

  * win32 based systems:
    1. md build
    2. cd build
    3. cmake ..
    4. open project files and compile them

Closing Remarks
=====================

If you need help, want to give some suggestions or just want to talk, feel free to join our IRC channel #desura on irc.freenode.net.
