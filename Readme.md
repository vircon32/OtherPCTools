# Vircon32: Other PC Tools

This is a second repository for Vircon32 computer programs, separate from ComputerSoftware. This one, instead, is intended to contain miscellaneous tools that are related to Vircon32 but not part of the main console programs.

Some of the projects contained here may be related to some particular Vircon32 game or program, such as tools to save program data in a format specific to a single game.

Most of these programs are written in C++ version 2011, and use portable libraries (SDL, OpenGL, OpenAL) along with CMake in an attempt to support multiplatform compilation.

----------------------------------
### Where to learn about Vircon32?

If you are not familiar with Vircon32, a 32-bit virtual game console, you can visit its website [www.vircon32.com](http://www.vircon32.com "Vircon32 Homepage") for more information. There you will also find everything you may need to try out the console and to make games for it.

--------------------------------------------
### How to build these programs from source?

The C++ projects in this repository are prepared to compile using CMake. The CMake scripts do try to support (at least) building under Windows, Linux and Mac. Note that, under Windows, CMake defaults to building with Visual C++, but some programs may expect to use MinGW/MSYS2 toolchains instead.
