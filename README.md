# Supercell Coding Test
Hi! This is the base code structure for the Supercell coding test.
The code below is a very basic and intensionally broken version of a generic survivor game.

## Disclaimer

We strongly recommend that you:
- Place assets in the `/assets` folder and use the `ResourceManager` class to get file paths.
    - Example usage: `m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf"))`
    - Look in `/src/Game.cpp` for examples of loading fonts and textures.
- Use CMake to build your project and do not modify the CMakeLists.txt file.
- Test on different platforms to assure your submission builds and functions the same.

We will build your project with the build steps outlined below. If your submission produces any build errors we will most likely end the evaluation there.

## Prerequisites

NOTE: Hive computers (should) have these dependencies installed already.

You must have [CMake](https://cmake.org/) version 3.22.1 or later installed to build this project.

If you are on Linux, make sure to install SFML's dependencies. For Debian-based distros:
```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

## Building

Simply run `cmake -B build` and `cmake --build build`. This builds the executable in `./build/bin/`.

The exact location of the executable may differ depending on your system and compiler. Example locations are `./build/bin/survive` and `./build/bin/Debug/survive.exe`.

Alternatively, you can specify a generator suited for your toolchain.
- `cmake -B build -G"Unix Makefiles"`
- `cmake -B build -G"Visual Studio 17 2022"`
- `cmake -B build -GXcode`

Run `cmake -G` to list all available generators.
