## 3Dzavr SDL

#### About:

3Dzavr is a 3D engine that uses SDL as multimedia framework.

Basic functionality:
* Operations with vectors, matrices, triangles etc.
* Projections from 3D on 2D plane, movement and rotation of camera and meshes.
* Advanced smooth animations by using Bézier curves for meshes and camera
* Collision detection (GJK)


## Installation (Linux):

0) Update apt packet manager: <code>sudo apt update</code>
1) Install gcc and c++ compilers: <code>sudo apt install gcc</code> and <code>sudo apt install g++</code>
2) Install SDL2: <code>sudo apt-get install libsdl2-dev</code>
3) Install SDL 2 TTF (for working with fonts and text): <code>sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y;</code>
4) Install libpng library: <code>sudo apt-get install libpng-dev</code>
5) Install pkg-config: <code>sudo apt-get install -y pkg-config</code>
6) Clone this repository into the folder you want to work with: <code>git clone https://github.com/vectozavr/3dzavr</code>

### If you want to use CLion (recommended)

7) Install CLion: <code>sudo snap install clion --classic</code>
8) Build and run the project.

### If you don't want to use IDE or you don't want to spend your time on configuring it
9) Install make: <code>brew install make</code>
10) Go into the engine folder: <code>cd 3dzavr</code>
11) Run make utility: <code>cmake .</code>
12) Compile the project: <code>make</code>
13) Run the compiled project: <code>./3dzavr</code>

## Installation (MacOS):

0) Install brew: <code>/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" </code>

1) Install developer tools: <code>xcode-select --install</code>
2) Install SDL 2: <code>brew install sdl2</code>
3) Install SDL 2 TTF (for working with fonts and text): <code>brew install sdl2_ttf</code>
4) Install libpng library: <code>brew install libpng</code>
5) Clone this repository into the folder you want to work with: <code>git clone https://github.com/vectozavr/3dzavr</code>

### If you want to use CLion (recommended)

6) Download and Install CLion: <code>brew install clion</code>
7) Build and run the project.

### If you don't want to use IDE or you don't want to spend your time on configuring it
8) Install make: <code>brew install make</code>
9) Go into the engine folder: <code>cd 3dzavr</code>
10) Run make utility: <code>cmake .</code>
11) Compile the project: <code>make</code>
12) Run the compiled project: <code>./3dzavr</code>

## Installation (Windows):

TODO: Needs to be written

### If you want to use Visual Studio
1) [Download and Install Visual Studio](https://visualstudio.microsoft.com/)
2) Follow the [tutorial for how to run CMake project in Visual Studio](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
