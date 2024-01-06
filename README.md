# brick-breaker

## Yet another brick breaker game

It's a 3D game made from scratch with C++ as language and OpenGL for the graphics.

## Purpose

I made this to showcase my knowledge in 3D graphics and rendering, especially with a low-level graphics API like
OpenGL, and also to show my programming abilities. It's also for a university project. Neither the engine, nor the
game built on top of it is too large or complex, but I think it's enough. The engine code is derived from an older
version of my private C++ game engine, but modified a lot in some parts. It incorporates a few other third party
technologies than my private engine. The third party libraries are mostly unrelated to actual graphics rendering.

I had lots of fun making this game.

## Structure

The project is structured in modules (or subprojects), where each module is either a C++ library or executable.

This is how the project looks like as of commit baf4c52:

```txt
brick-breaker/
├── clients
│   ├── brick-breaker
│   │   ├── CMakeLists.txt
│   │   ├── extern
│   │   ├── README.md
│   │   └── src
│   │       ├── source1
│   │       ├── source2
│   │       └── source3
│   ├── CMakeLists.txt
│   ├── minimal
│   │   ├── CMakeLists.txt
│   │   ├── README.md
│   │   └── src
│   │       ├── source1
│   │       ├── source2
│   │       └── source3
│   └── teapot
│       ├── CMakeLists.txt
│       ├── README.md
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
│   ├── README.md
│   └── src
│       ├── source1
│       ├── source2
│       └── source3
├── gallery
│   ├── image1
│   └── image2
├── README.md
└── scripts
    ├── script1
    └── script2

27 directories, 175 files
```

*Note that there are multiple Markdown files containing the documentation.*

With the engine as a platform for building games with C++, the `clients` directory could contain many more subprojects,
as it's quite easy to use the engine library, but I didn't want to waste too much time on other applications, even
though they could showcase the engine's features much better.

I first wrote a test application named `teapot`, that doesn't contain many objects or much lighting.

There is also a minimal application.

Then there's the actual brick breaker game in the directory `brick-breaker`.

Assets for the applications are located in the `data` directory.

## Building

The engine is made to work on `Linux` and `Windows`. I tested the game on `Fedora Linux 39 (Workstation Edition)`
(`GNOME` environment, `X11` system) and on `Microsoft Windows 10`, on an `AMD Ryzen™ 5 1600 × 12` CPU and
`NVIDIA GeForce GTX 1050 Ti` GPU. And I also tested it on an old `Xubuntu Linux` laptop with an `Intel® Core™ i5-4200U`
CPU and an integrated `Intel® HD Graphics 4400` GPU.

The project requires `C++17` and `GCC` or `MSVC` compilers. I tested it on `GCC 13.2` and `MSVC 19.36`.

To download the project, run:

```sh
git clone https://github.com/SimonMaracine/brick-breaker/
cd brick-breaker
git submodule update --init
```

The whole project is setup by running `setup.sh` or `setup.bat` from scripts and it can be built by invoking `build.sh`
or by building from Visual Studio.

Note that for building the whole thing, you need to have installed the required packages (dependent of Linux
distribution) or to have installed the required development toolkit(s) on Windows.

Build in release mode to have a good play experience.
