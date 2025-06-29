@echo off
g++ main.cpp -o game.exe ^
  -IF:\1. PROJECTS\OOP - Pussy Invaders\Setup\vcpkg\installed\x64-mingw-static\include ^
  -LF:\1. PROJECTS\OOP - Pussy Invaders\Setup\vcpkg\installed\x64-mingw-static\lib ^
  -lraylib -lopengl32 -lgdi32 -lwinmm
pause
