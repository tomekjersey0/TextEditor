# Minimal Terminal Text Editor

This project is a small but extensible experiment: a **minimal text editor built with C++ and ncurses**.
It implements basic buffer management, cursor movement, and keybinding logic, with a custom vector class and rudimentary data structures.

While intentionally lightweight, the project is structured in a way that makes it easy to expand into something more fully featured later.

---

## Features

* **Custom dynamic array implementation (`Vector<T>`)**
  A simplified reimplementation of a vector-like container for educational purposes.

* **Line & File abstractions**

  * `Line`: Manages a sequence of characters with insertion, deletion, and cursor tracking.
  * `File`: Holds multiple `Line` objects, handles cursor navigation between lines, and supports editing operations.

* **Keyboard handling**

  * Special keys (arrows, Enter, Backspace, Home, End, Escape) are mapped through a `KeyAction` system.
  * A lookup table (`std::unordered_map`) allows efficient dispatch of key events.

* **Basic terminal UI (ncurses)**

  * Text buffer is drawn to the screen.
  * Cursor highlighting (reverse colors at the current position).
  * Handles end-of-line cursor position properly.
  * Screen refreshes cleanly with each keypress.

* **Exit with `Escape` key**

---

## Requirements

* **C++17 or newer**
* **ncurses library**

  * On Linux: install with your package manager (`sudo apt install libncurses5-dev libncursesw5-dev`).
  * On macOS: `brew install ncurses`.
  * On Windows: use [PDCurses](https://pdcurses.org/) or WSL with ncurses installed.

---

## Building

### Linux / macOS

```bash
g++ -std=c++17 -lncurses main.cpp -o editor
./editor
```

### Windows (MinGW + PDCurses)

1. Install PDCurses (clone or download).
2. Build PDCurses and link against `pdcurses.a`.
3. Compile with:

   ```bash
   g++ -std=c++17 main.cpp -o editor -I/path/to/pdcurses -L/path/to/pdcurses -lpdcurses
   ```
4. Run:

   ```bash
   ./editor
   ```

---

## Usage

* **Arrow keys**: Move cursor
* **Enter**: Insert a new line
* **Backspace**: Delete character or merge lines
* **Home / End**: Jump to start/end of line
* **Escape**: Quit

---

## Project Structure

* `Vector<T>`: Educational reimplementation of a dynamic array.
* `Line`: Represents a single line of text with editing functionality.
* `File`: Manages a list of `Line` objects (the document buffer).
* `KeyAction` + `Mapping`: Encapsulate keyboard events and handlers.
* `Global`: Stores shared state like the current file and key lookup.
* `main`: Initializes ncurses, installs keybindings, and runs the main loop.

---

## Next Steps (Possible Extensions)

* File I/O (open/save text files).
* Undo/redo stack.
* Syntax highlighting.
* Configurable keymaps.
* Status bar with cursor position and mode (like Vim).
* Switching between multiple buffers.

---

## Notes

* This project intentionally reimplements basic data structures (like `Vector`) for learning. In production code, `std::vector` would be used instead.
* Memory management is raw (manual `new`/`delete`). Later extensions could replace this with smart pointers (`std::unique_ptr`) for safety.
* The code favors clarity over optimization: it’s meant to be a foundation, not yet a polished editor.

---