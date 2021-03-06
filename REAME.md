# TChecker

## Description

This is TChecker, an *open-source* model-checker for timed systems. TChecker is
written in C++17 and provides a library of classes to describe timed systems,
to compute the corresponding timed automata, to compute its semantics, as well
as symbolic representations and algorithms to check requirements over timed
systems. It also comes with tools to run these algorithms from the command
line.

TChecker originates and is still lead by academic research on the verification
of timed systems. It is intended to serve as a plateform to experiment new data
structures and algorithms for the verification and the synthesis of timed
systems. The goal of the project is to implement state-of-the-art algorithms as
well as benchmarks to evaluate these algorithms, and to share then with the
community.

## Installation

Please, notice that TChecker has only been tested on Linux and Mac OS X. The
installation instructions below may not work for other operating systems.

### Requirements

TChecker depends on several softwares and libraries listed below. Linux users
can install these softwares using the packaging system shipped along with their
distribution. Mac OS X users can install the required software using an
external packaging system like [Brew](https://brew.sh/) or
[MacPorts](https://www.macports.org/).

* a C++ compiler with decent C++17 support (Clang >= 3.6 or GNU g++ >= 6
should work. Apple LLVM >= 10.0.0 works)
* CMake (>= 2.8)
* flex (>= 2.5.35)
* bison (>= 3.0.4)
* The Boost library (>= 1.65.0 -- probably works with earlier versions)
* Doxygen (>= 1.8.15 -- probably works with earlier versions)

### Building and installing TChecker

We recommend building TChecker in a dedicated directory, not in the source
directory itself. This allows to easily remove files generated by CMake and
by the compiler. This also allows CMake to build a project for your favorite
IDE, out of the source directory. We describe the installation process
following this recommendation, using make as a build system.

1. Create a directory to contain both the source directory and the build
directory: `mkdir tchecker-src`, then enter the new directory.
1. Clone the project `git clone https://github.com/fredher/tchecker.git`
in directory `tchecker-src`.
1. Create the build directory, still in directory `tchecker-src`: `mkdir build`
1. Then, enter the build directory `cd build` and follow the next steps:
   1. Configure the build system: `cmake ../tchecker` (where `../tchecker` is
the directory containing the source code created by `git clone`). You can use
the `-G` flag to generate a project for your favorite build system/IDE
(`Makefile` is the default), and option `-D` to define `cmake` variables
(see [CMake documentation](https://cmake.org/documentation/)). In particular,
you can build the *Debug* version or the *Release* version of TChecker by
setting option `CMAKE_BUILD_TYPE`. The installation directory is set using
option `CMAKE_INSTALL_PREFIX`. The path to required software can be specified
using option `CMAKE_PREFIX_PATH` if needed.
   1. Compile the project: `make` (use option `-j` for parallel build)
   1. Build the documentation: `make doc`
   1. Finally, install TChecker: `make install`

The installation procedure creates four directories: `bin`, `lib`, `include`
and `share/doc/tchecker/html` in the installation directory (default is
`/usr/local`). The TChecker tool can be found in directory `bin` (see below
for usage). The development tools are provided in the other directories: the
headers in `include`, the library in `lib`, and the Doxygen documentation in
`share/doc/tchecker/html`.

## Usage

TChecker can be used either as a tool to check timed systems, or as a library
to extend its functionalities.

### TChecker: the tool

The tool `tchecker` is installed in the subdirectory `bin` of your installation
directory (default is `/usr/local`). Simply launch the command `tchecker` to
get usage documentation.

Please, refer to the wiki for a description of the input file format. Examples
are provided in the subdirectory `examples` of the repository.

### TChecker: the library

TChecker can be used as a library to extend its functionalities and implement
new algorithms. The Doxygen documentation of the library is installed in
the subdirectory `share/doc/tchecker/html` of your installation path (default
is `/usr/local`). The library itself is in the directory `lib`. Include files
are installed in `include`. Refer to the documentation and TChecker source
code on how to use the library to implement new ideas.

## Credits

The authors of the project are credited in the file AUTHORS in the repository.

## License

TChecker is published under the MIT license reproduced below (and in the file
LICENSE in the repository).

MIT License

Copyright (c) 2019 Bordeaux INP

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
