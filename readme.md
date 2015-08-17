# libuhs
[![Build Status](https://travis-ci.org/Thoronador/libuhs.svg)](https://travis-ci.org/Thoronador/libuhs)

libuhs will be a library for reading UHS files.
Currently it is still in a very early phase and is not able to read any such files yet.

## Building from source

### Prerequisites

To build libuhs from source you need a C++ compiler and CMake 2.8 or later.
It also helps to have Git, a distributed version control system, on your build
system to get the latest source code directly from the Git repository.

All three can usually be installed be typing

    apt-get install cmake g++ git

or

    yum install cmake gcc-c++ git

into a root terminal.

### Getting the source code

Get the source directly from Git by cloning the Git repository and change to
the directory after the repository is completely cloned:

    git clone https://github.com/Thoronador/libuhs.git ./libuhs
    cd libuhs

That's it, you should now have the current source code of libuhs on your machine.

### Build process

The build process is relatively easy, because CMake does all the preparations.
Starting in the root directory of the source, you can do the following steps:

    mkdir build
    cd build
    cmake ../
    make -j2

In its current state, the code is only able to build a test case for the library,
no realy binary will be built. This will change in the future.

## Copyright and Licensing

Copyright 2012-2015 Thoronador

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Credits
Special thanks to Stefan Wolff, who published some insightful information about
the UHS file format on <http://www.swolff.dk/uhs/>.
