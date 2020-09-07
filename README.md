# Pixelate
Pixelate is a interactive application that is used to create 2D games currently being made for Desktop PC platforms
# How to setup

**Pixelate can only be built on Windows 10 - specifically a 64bit version as of now**

Visual studio is recommended preferably version 2019 although 2017 should be suffice too. Pixelate uses Premake5 as it's build system.

To build a visual studio 2019 solution:
```
scripts/Win_BuildProj.bat
```


In order to build a visual studio 2017 solution you will need to edit the .bat file from:
```
@echo off
pushd %~dp0\..\
call vendor\binary\premake5.exe vs2019
popd
PAUSE
```

to:
```
@echo off
pushd %~dp0\..\
call vendor\binary\premake5.exe vs2017
popd
PAUSE
```


# Pixelate's features
* A fast 2D renderer
* Can load and play audio
* A 2D physics solution
* A ECS system
* CSharp scriping, using mono
* A Editor with a 2D scene and game viewer, inspector and hierarchy
* Saving and loading scenes

# Pixelate's goals
* Animation, UI and a particle system.
* A blueprints visual scripting type solution to use an an alternative way to script entites.
* Multithreading support for rendering and audio.
* Support for other platforms.


# Personal goal
Pixelate was made in mind to educate myself about game engine architecture. In the past I have attempted to make my own game engine but this is my first attempt at creating a game engine using books and resources from the internet and not creating some sort of framework. With this in mind, if you want to create a 2D game using Pixelate. At this moment in time. Use either Unity or Unreal or some other popular game engine! I currently make Pixelate in my spare time and cannot work on it full time.
