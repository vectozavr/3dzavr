## 🚀 3Dzavr - Light C++ Game Engine

<img src="https://github.com/vectozavr/3dzavr/assets/60608292/9b411b45-9e63-4a55-a8d1-924579ae504c" width="300">

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://choosealicense.com/licenses/mit/)
[![Community Discord](https://img.shields.io/discord/788056426324426782)](https://discord.gg/zJUxVWZnkZ)
![Commits](https://img.shields.io/github/last-commit/Vectozavr/3dzavr/sdl_version)
![Issues](https://img.shields.io/github/issues/Vectozavr/3dzavr)
![Repo size](https://img.shields.io/github/repo-size/Vectozavr/3dzavr)
![version](https://img.shields.io/github/v/tag/Vectozavr/3dzavr)

#### About:

3Dzavr is yet another 3D engine for C++ that uses SDL as multimedia framework.

#### Motivation:

Initially this project was created as a Proof Of Concept in implementing <b>custom CPU render from scratch</b>. <br>
Gradually, 3dzavr became more like a very simple version of big engines like Godot, Unity or Unreal. <br>
Our goal is to create light and easy to use/study/contribute Open Source and free engine, which anyone can access and use in whatever purposes. <br>
<b>But the main goal is to have fun! You are welcome to use and contribute!</b>

Main functionality:
* Operations with vectors, matrices, triangles etc
* Projections from 3D on 2D plane, movement and rotation of camera and meshes
* The custom light component system
* The system of attachments and hierarchy tree
* Advanced smooth animations by using Bézier curves for meshes and camera
* Collision detection (GJK) and resolution (EPA)
* Resource manager and other useful utils

<img src="https://github.com/vectozavr/3dzavr/assets/60608292/fb69b744-6057-4829-8c69-67e553b19521" width="500">

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
