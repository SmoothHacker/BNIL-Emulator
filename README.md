# BNIL-Emulator

This is an emulator for Binary Ninja's Low Level Intermediate Language (MLIL). 

## Notice
The project currently requires a Commercial license due to the fact that it
runs as an executable and not a plugin. It is planned to port this to a plugin
so that personal license users can leverage this project.

## Building

Simply configure the project using CMake, then build:

```sh
cmake -S . -B build # -GNinja ...
cmake --build build
```
