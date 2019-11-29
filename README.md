# Picross engine

Master: [![Build Status](https://travis-ci.org/deqyra/PicrossEngine.svg?branch=master)](https://travis-ci.org/deqyra/PicrossEngine)  
Staging: [![Build Status](https://travis-ci.org/deqyra/PicrossEngine.svg?branch=staging)](https://travis-ci.org/deqyra/PicrossEngine)  

A small CLI program aiming at allowing the user to create picross (nonogram) grids and manipulate them, save them, load them, solve them, etc.  

## Quick roadmap  

- ✔ Complete game representation  
- ✔ Save/load XML games  
- ✔ CLI display capabilities  
- ✔ Interactive CLI app  
- ☐ Iterative solver  
- ☐ Inferring solver  

## Project state

- ✔ Git repo  
- ✔ Tests  
- ✔ Continuous integration

## Usage

### Building the project

- Install [CMake](https://cmake.org/)
- Make sure your compiler is C++17-compliant.
- Open a command line, go to a directory of your choice and do the following:
- `git clone https://github.com/deqyra/PicrossEngine.git`
- `cd PicrossEngine`
- `mkdir build && cd build`
- `cmake ..`

You can customize the generated buildsystem by running `cmake .. -G <generator>`, replacing `<generator>` with the buildsystem you want. Use `cmake -G` to get a complete list. Here are some of them: _(include the double-quotes in the command line)_  

- `"Unix Makefiles"` for most Unix-based systems
- `"MinGW Makefiles"` for a MinGW toolchain (make sure `where g++` returns something)
- `"MSYS Makefiles"` for a MSYS installation (make sure `where g++` returns something)
- `"Visual Studio <ver> <year>" [-A <arch>]` for Visual Studio solutions

If you generated a makefile-based buildsystem, simply run `make` afterwards.

### Troubleshooting

#### CMake "CXX compiler identification is unknown" on Windows

If CMake complains that `cl is not a full path and was not found in the PATH`:  
* Install the [Visual C++ Build Tools 2015](https://go.microsoft.com/fwlink/?LinkId=691126)
* Make sure `where cl` returns something before retrying
* Alternatively, if MinGW is installed on your system, you can go for it and run `cmake .. -G "MinGW Makefiles"`.  

If CMake complains the same as above but for `g++`:
* Make sure MinGW is properly installed and your PATH is set to point to its `bin` folder (mostly likely something along the lines of `C:\MinGW\bin`).
* `where g++` should return something before you retry.

#### CMake can generate MSYS makefiles but running those fails

Not investigated yet.

#### MinGW linker complains about `vsnprintf` being defined multiple times

Open the MinGW installation manager and install packages `mingw32-libmingwex-*`.

#### Windows executable produced with MinGW displays an error dialog

"The procedure entry point ... could not be located in the dynamic link library ..."  
Copy `C:\MinGW\bin\libstdc++-6.dll` (or similar) into the same folder as the executable. If you don't have it, one is provided in the [v1.0 release](https://github.com/deqyra/PicrossEngine/releases/tag/v1.0).

**If you encounter an issue that is not filed here, please open an issue and I will have a look.  
If you have a solution, please edit this README and open a pull request with your changes.**