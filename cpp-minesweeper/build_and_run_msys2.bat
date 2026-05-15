@echo off
set MSYS2_ROOT=C:\msys64
if not exist "%MSYS2_ROOT%\usr\bin\bash.exe" (
  echo MSYS2 not found at %MSYS2_ROOT%.
  echo Please install MSYS2 from https://www.msys2.org/ and re-run this script.
  pause
  exit /b 1
)

REM Use bash to update and install toolchain non-interactively, then compile and run
set MSYS_CMD="%MSYS2_ROOT%\usr\bin\bash.exe" -lc

n:: Update package database and upgrade (may take a while)
%MSYS_CMD% "pacman -Syu --noconfirm"

n:: Install mingw-w64 toolchain if not installed
%MSYS_CMD% "pacman -S --needed --noconfirm mingw-w64-x86_64-toolchain"

n:: Compile the project (use MinGW64 g++)
%MSYS_CMD% "cd /d/Users/34745/Desktop/analysis/02-DeepSeek-AI/小游戏/cpp-minesweeper && g++ -std=c++17 -O2 main.cpp -o minesweeper.exe"

n:: Run the executable from MSYS2 bash
%MSYS_CMD% "cd /d/Users/34745/Desktop/analysis/02-DeepSeek-AI/小游戏/cpp-minesweeper && ./minesweeper.exe"

necho Done.
pause
