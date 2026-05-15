# 💣 扫雷游戏 Minesweeper

一个跨平台、多语言实现的扫雷游戏集合，包含 **Python (Pygame)**、**C++ (Qt 图形界面)** 和 **HTML/CSS/JS (网页版)** 三种版本。

---

## 📦 项目结构

```
minesweeper/
├── scanmines.py                 # Python Pygame 图形版
├── scanmines.html               # HTML/CSS/JS 网页版（双击浏览器打开）
├── cpp-minesweeper/
│   └── qt-minesweeper/
│       ├── main.cpp             # C++ Qt6 图形界面版
│       └── CMakeLists.txt       # Qt 版 CMake 构建
├── .gitignore
└── README.md
```

---

## 🎮 版本说明与运行方式

### 1. Python Pygame 版（`scanmines.py`）

功能完整，包含计时器、步数统计、左右键操作、重启按钮。

```powershell
cd scanmines.py 所在目录
python scanmines.py
```

> 依赖：`pygame`（`pip install pygame`）

### 2. 网页版（`scanmines.html`）

无需安装，双击 `scanmines.html` 用浏览器打开即可游玩。

### 3. C++ Qt6 图形界面版（`cpp-minesweeper/qt-minesweeper/`）

基于 Qt6 Widgets 的图形扫雷，支持鼠标左右键操作、计时器、重启按钮。

**编译运行（MSYS2 + MinGW + Qt6）：**

```powershell
cd cpp-minesweeper/qt-minesweeper
C:\msys64\mingw64\bin\g++ -std=c++17 ^
  -I"C:\msys64\mingw64\include\Qt6" -I"C:\msys64\mingw64\include" ^
  -I"C:\msys64\mingw64\include\Qt6\QtCore" -I"C:\msys64\mingw64\include\Qt6\QtGui" ^
  -I"C:\msys64\mingw64\include\Qt6\QtWidgets" ^
  -o qt_minesweeper.exe main.cpp ^
  -L"C:\msys64\mingw64\lib" -lQt6Core -lQt6Gui -lQt6Widgets -lglu32 -lopengl32 ^
  -Wl,-subsystem,windows -Wl,--allow-multiple-definition
.\qt_minesweeper.exe
```

> 需要先通过 MSYS2 安装 Qt6：`pacman -S mingw-w64-x86_64-qt6-base mingw-w64-x86_64-qt6-tools`

---

## 🛠 技术栈

| 版本 | 语言 | 框架/库 | 平台 |
|------|------|---------|------|
| Pygame 版 | Python | Pygame 2.x | 跨平台 |
| 网页版 | HTML/CSS/JS | — | 浏览器 |
| Qt 版 | C++17 | Qt6 Widgets | Windows（需 Qt6 环境） |

---

## 📄 许可

本项目采用 MIT 许可证。
