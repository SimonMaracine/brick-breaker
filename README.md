# brick-breaker

## Yet another brick breaker game

It's a 3D game made from scratch with C++ as language and OpenGL for the graphics.

## Purpose

I made this to showcase my knowledge in 3D graphics and rendering, especially with a low-level graphics API like
OpenGL, and also to show my programming abilities. I also had lots of fun making the game. Neither the engine, nor the
game built on top of it is too large or complex, but I think it's enough. The engine code is derived from an older
version of my private C++ game engine, but modified a lot in some parts. It incorporates a few different third party
technologies. The third party libraries are mostly unrelated to actual graphics rendering.

## Structure

The project is strucured in modules (or subprojects), where each module is either a C++ library or executable.

    As of commit 8734f67:

    brick-breaker/
    ├── clients
    │   ├── brick-breaker
    │   │   ├── CMakeLists.txt
    │   │   ├── extern
    │   │   └── src
    │   │       ├── source1
    │   │       ├── source2
    │   │       └── source3
    │   ├── CMakeLists.txt
    │   └── teapot
    │       ├── CMakeLists.txt
    │       └── src
    │           ├── source1
    │           ├── source2
    │           └── source3
    ├── CMakeLists.txt
    ├── data
    │   ├── fonts
    │   ├── levels
    │   ├── models
    │   ├── shaders
    │   ├── sounds
    │   └── textures
    ├── engine
    │   ├── CMakeLists.txt
    │   ├── extern
    │   └── src
    │       ├── source1
    │       ├── source2
    │       └── source3
    └── scripts
        ├── scripts1
        └── scripts2

    22 directories, 147 files

With the engine as a platform for building games with C++, the clients directory could contain many more subprojects,
as it's quite easy to use the engine library, but I didn't want to waste too much time on other applications, even
though they could showcase the engine's features much better.

I first wrote a test application named `teapot`, that doesn't contain many objects or much lighting.

Then there's the actual brick breaker game in the directory `brick-breaker`.

Assets for the applications are located in the `data` directory.

## Building

The engine is made to work on `Linux` and `Windows`. I tested the game on `Fedora Linux 39 (Workstation Edition)` and on
`Microsoft Windows 10`, on an above average CPU and GPU model. The project requires `C++17` and `GCC` or `MSVC`
compilers (`GCC 13.2` or `MSVC 19.36`).

The whole project is setup by running `setup.sh` or `setup.bat` from scripts and it can be built by invoking `build.sh`
or by building from Visual Studio.
