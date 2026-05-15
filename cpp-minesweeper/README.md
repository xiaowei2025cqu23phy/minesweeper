# C++ Console Minesweeper

A simple terminal-based Minesweeper implemented in modern C++ (single-file).

## Build
Requires a C++17 compiler.

Using g++:

```bash
g++ -std=c++17 -O2 main.cpp -o minesweeper
```

Or using CMake:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Run

```bash
./minesweeper
```

## Controls
- `r row col` — reveal cell at (row, col)
- `f row col` — toggle flag at (row, col)
- `restart` — restart game
- `quit` or `q` — exit

Rows and columns are 0-indexed. The grid displays `#` for hidden, `F` for flagged, `*` for mine, numbers for adjacent mine counts.

## Notes
This is a simple console version intended as a starting point. If you want a graphical version (SDL2/SFML/Qt), I can scaffold it next.
