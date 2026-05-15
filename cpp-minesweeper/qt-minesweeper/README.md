# C++ Qt6 扫雷

基于 Qt6 Widgets 的图形界面扫雷游戏，支持鼠标左右键操作、计时器、重启。

## 编译运行

### 方法一：使用 Qt Creator
直接用 Qt Creator 打开本文件夹，配置好 Qt6 Kit 后构建运行。

### 方法二：MSYS2 + MinGW 命令行

```powershell
mkdir build && cd build
cmake -S .. -B . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/msys64/mingw64"
mingw32-make
./qt_minesweeper.exe
```

> 前提：已通过 MSYS2 安装 Qt6：`pacman -S mingw-w64-x86_64-qt6-base mingw-w64-x86_64-qt6-tools`

### 方法三：直接 g++ 编译

```powershell
g++ -std=c++17 ^
  -I"C:\msys64\mingw64\include\Qt6" -I"C:\msys64\mingw64\include" ^
  -I"C:\msys64\mingw64\include\Qt6\QtCore" -I"C:\msys64\mingw64\include\Qt6\QtGui" ^
  -I"C:\msys64\mingw64\include\Qt6\QtWidgets" ^
  -o qt_minesweeper.exe main.cpp ^
  -L"C:\msys64\mingw64\lib" -lQt6Core -lQt6Gui -lQt6Widgets -lglu32 -lopengl32 ^
  -Wl,-subsystem,windows -Wl,--allow-multiple-definition
```

## 操作

- **左键**：揭示格子
- **右键**：插旗/取消插旗
- **Restart 按钮**：重新开始
