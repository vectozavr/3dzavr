## 3Dzavr SDL

#### About:

Simple 3d engine that uses SDL as multimedia framework.

Basic functionality:
* Operations with vectors, matrices, triangles etc.
* Projections from 3D on 2D plane, movement and rotation of camera and meshes.
* Advanced smooth animations by using Bézier curves for meshes and camera
* Collision detection (GJK)

## Installation (MacOS):

0) Install brew: <code>/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" </code>

1) Install developer tools: <code>xcode-select --install</code>
2) Install SDL 2: <code>brew install sdl2</code>
3) Install SDL 2 TTF (for working with fonts and text): <code>brew install sdl2_ttf</code>
4) Install libpng library: <code>brew install libpng</code>
5) Clone this repository into the folder you want to work with: <code>git clone https://github.com/vectozavr/3dzavr/tree/sdl_version </code>

### If you want to use CLion (recommended)

6) Download and Install CLion: <code>brew install clion</code>
7) Configure System Clang compiler in CLion:
   
<b>Preferences --> Build, Execution, Deployment --> Toolchain</b>

and move System Clang by <b>^</b> to the top of the list or choose your favorite compiler.

### If you want to use Visual Studio
8) [Download and Install Visual Studio](https://visualstudio.microsoft.com/)
9) Follow the [tutorial for how to run CMake project in Visual Studio](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)

### If you don't want to use IDE or you don't want to spend your time on configuring it
10) Install make: <code>brew install make</code>
11) Go into the engine folder: <code>cd 3dzavr</code>
12) Run make utility: <code>cmake .</code>
13) Compile the project: <code>make</code>
14) Run the compiled project: <code>./3dzavr</code>