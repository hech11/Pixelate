<h1 align="center">
  <a href="#"><img src="/Pixelate-Editor/resources/icons/window/Pixelate_Logo.png" width="250"/>
  <br>
  Pixelate
</h1>

<h3 align="center">
2D Game Engine
</h3>

<blockquote align="center">
Pixelate is a game engine that is used to create 2D games. Currently Pixelate is being developed for desktop PC platforms.
</blockquote>
  
<p align="center">
  <a href="#how-to-setup">How to setup</a>
  |
  <a href="#pixelates-features">Features</a>
  |
  <a href="https://trello.com/b/a7X6x8LT/pixelate">Goals/Roadmap</a>
  |
  <a href="#platform-support">Platform support</a>
  |
  <a href="#personal-goal">Personal goal</a>
</p>

How to setup
-------------

  
**This codebase has not been compiled on other compilers such as GCC or Clang nor has it been tested on other IDEs**
<br>
Visual studio is recommended preferably version 2019 although 2017 should be suffice too
Pixelate uses Premake5 as it's build system.
<br>
<br>
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


## Pixelate's features
* A fast 2D renderer
* Can load and play audio
* A 2D physics solution
* A ECS system
* CSharp scriping, using mono
* A Editor with a 2D scene and game viewer, inspector and hierarchy
* Saving and loading scenes

  

## Platform support
| Windows 10-64bit | Mac | Linux |
| --- | --- | --- |
| :heavy_check_mark: | :x: | :x: |
  
<blockquote>
In the future I would like to support other desktop platforms and possibly mobile platforms.
For now I'm focusing my efforts to provide a stable Windows 10 build.
</blockquote>
  
## GPU support
| AMD | Nvidia | Intel |
| --- | --- | --- |
| :heavy_check_mark: | :grey_question: | :heavy_check_mark: |
<blockquote>
I do not have access to a Nvidia GPU so I cannot verify how stable builds that use those GPUs will be.
</blockquote>



## Personal goal
Pixelate was made in mind to educate myself about game engine architecture. In the past I have attempted to make my own game engine but this is my first attempt at creating a game engine using books and resources from the internet and not creating some sort of framework. With this in mind, if you want to create a 2D game using Pixelate. At this moment in time. Use either Unity or Unreal or some other popular game engine! I currently make Pixelate in my spare time and cannot work on it full time.
