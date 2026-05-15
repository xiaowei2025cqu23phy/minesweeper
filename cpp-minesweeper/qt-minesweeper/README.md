# Qt Minesweeper (C++)

Build and run (Windows) — examples:

1) Using Qt Creator: open the folder and build/run with the configured Qt kit.

2) Using CMake + Qt (MSYS2 / MinGW or other):

```powershell
mkdir build && cd build
cmake -S .. -B . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/msys64/mingw64" 
mingw32-make
./qt_minesweeper.exe
```

Adjust `CMAKE_PREFIX_PATH` to where Qt is installed (Qt6 for this CMakeLists).
