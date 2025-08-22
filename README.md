# TextEditor (C++ + Curses)

A simple terminal-based text editor written in modern C++17 using the curses library (`ncurses` on Linux/macOS, `pdcurses` on Windows).

---

## Features (so far)

- Basic text buffer with multiple lines
- Cursor navigation
- Character insertion and deletion
- Curses-based UI

---

## Dependencies

- **CMake ≥ 3.25**
- **C++17 compiler** (MSVC, Clang, or GCC)
- **Curses library**
  - Linux/macOS: [ncurses](https://invisible-island.net/ncurses/)
  - Windows: [PDCurses](https://pdcurses.sourceforge.io/) (recommended via [vcpkg](https://github.com/microsoft/vcpkg))

---

## Build Instructions

### Linux / macOS

Make sure ncurses is installed:

```bash
# Ubuntu / Debian
sudo apt-get install libncurses5-dev libncursesw5-dev

# Fedora
sudo dnf install ncurses-devel

# macOS (with Homebrew)
brew install ncurses
```

Then build:

```bash
git clone https://github.com/yourname/TextEditor.git
cd TextEditor
mkdir build && cd build
cmake ..
make
./TextEditor
```

---

### Windows (with vcpkg)

Install [vcpkg](https://github.com/microsoft/vcpkg) if not already done:

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Install PDCurses:

```powershell
.\vcpkg install pdcurses
```

Configure and build with CMake:

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

Run the editor:

```powershell
.\Release\TextEditor.exe
```

---

## Notes

* Cross-platform: works on Linux/macOS (ncurses) and Windows (pdcurses).
* Code style: modern C++17, modularized into classes for buffer management, input handling, etc.
* Development status: early stage — only basic editing and navigation implemented.

---

## Roadmap

* File open/save support
* Syntax highlighting
* Undo/redo
* Configurable key bindings
