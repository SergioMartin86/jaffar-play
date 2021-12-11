# Jaffar-Play 

![](jaffar.png)

Playback Tools for Jaffar, High-performance solver for Prince of Persia (DOS) tool-assisted speedrunning (https://github.com/SergioMartin86/jaffar)

Docker
===============

To get a ready-to-run Docker container with Jaffar, simply run:

```
docker run sergiom86/jaffar:latest
```

Installation
===============

Follow these steps to manually install in your system

1) Get code

 
```
  git clone --recursive https://github.com/SergioMartin86/jaffar-play.git
```
  
2) Create build directory


```
  cd jaffar
  mkdir build
  cd build
```

3) Configure and compile

```
  meson ..
  ninja
```
  
Requisites
============

- Meson - For compilation
- Ninja - For compilation
- SDL2: Jaffar has been tested using SDL2 2.0.14. 
- SDL2-image: Jaffar has been tested using SDL2_image 2.0.5.
- libpng
- libjpeg

Usage
=========

Show a snapshot of a given savestate. It will automatically update if the savestate is modified.

```
jaffar-show example.sav
```

Launches the solution Jaffar playback/editor

```
jaffar-play example.sav example.sol
```

Environment Variables:
------------------------

Indicate where the SDLpop root folder is located:

```
export SDLPOP_ROOT=$HOME/jaffar-play/extern/SDLPoP
```

[Optional] Indicate which level file to use. Default: "LEVELS.DAT", i.e., the original game.

```
export SDLPOP_LEVELS_FILE=$HOME/jaffar/examples/istaria/LEVELS.DAT
```

Indicates how often will jaffar-show check for updates in the savestate

```
export JAFFAR2_SHOW_UPDATE_EVERY_SECONDS=1
```

Authors
=============

- Sergio Martin (eien86)
  + Github: https://github.com/SergioMartin86
  + Twitch: https://www.twitch.tv/eien86
  + Youtube: https://www.youtube.com/channel/UCSXpK3d6vUq58fjgF5jFoKA
