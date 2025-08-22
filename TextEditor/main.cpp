#include <curses.h>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <string>


class KeyAction;
template <typename T>
class Vector;
class File;
class Global
{
public:
    static size_t cursorX, cursorY;
    static File file;
    static bool running;
    static std::map<Vector<int>, KeyAction> Global::specialKeys;
    static std::unordered_map<int, KeyAction*> Global::keyLookup;

public:
    static Global& getInstance()
    {
        static Global    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
private:
    Global() {}                    // Constructor? (the {} brackets) are needed here.
public:
    Global(Global const&) = delete;
    void operator=(Global const&) = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
};

bool Global::running = true;

template <typename T>
class Vector {
private:
    T* arr;
    size_t size;
    size_t cap;
    void grow() {
        size_t og = cap;
        if (size + 1 > cap) {
            cap = cap == 0 ? 1 : cap * 2;
        }
        if (og != cap) {
            T* copy = new T[cap];
            for (size_t i = 0; i < size; i++) {
                copy[i] = arr[i];
            }
            delete[] arr;
            arr = copy;
        }
    }
public:
    T* getArr() const {
        return arr;
    }
    size_t getSize() const {
        return size;
    }
    void setSize(size_t s) {
        size = s;
    }
    size_t getCap() const {
        return cap;
    }

    Vector() {
        arr = nullptr;
        size = 0;
        cap = 0;
    }
    ~Vector() {
        delete[] arr;
    }
    Vector(std::initializer_list<T> list) : arr(nullptr), size(0), cap(0) {
        for (const T& elem : list) {
            push(elem);
        }
    }
    Vector(const Vector& rhs) {
        arr = new T[rhs.cap];
        for (size_t i = 0; i < rhs.size; i++) {
            arr[i] = rhs.arr[i];
        }
        size = rhs.size;
        cap = rhs.cap;
    }
    Vector& operator=(const Vector& rhs) {
        if (this == &rhs) return *this;
        T* newArr = new T[rhs.cap];
        for (size_t i = 0; i < rhs.size; i++)
            newArr[i] = rhs.arr[i];
        delete[] arr;
        arr = newArr;
        size = rhs.size;
        cap = rhs.cap;
        return *this;
    }
    bool operator<(const Vector& rhs) const {
        size_t minSize = size < rhs.size ? size : rhs.size;
        for (size_t i = 0; i < minSize; ++i) {
            if (arr[i] < rhs.arr[i]) return true;
            if (arr[i] > rhs.arr[i]) return false;
        }
        return size < rhs.size; // shorter vector is "less" if all elements equal
    }


    void push(const T& value) {
        grow();
        arr[size] = value;
        size++;
    }
    T pop() {
        if (size > 0) {
            size--;
        }
        else {
            throw std::runtime_error("Popping from empty list.");
        }
        return arr[size];
    }
    void insert(size_t index, const T& value) {
        grow();
        if (index > size) throw std::runtime_error("Index out of range.");
        for (size_t i = size; i > index; i--) {
            arr[i] = arr[i - 1];
        }
        arr[index] = value;
        size++;
    }
    T erase(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range.");
        T removed = arr[index];
        for (size_t i = index; i < size - 1; i++)
            arr[i] = arr[i + 1];
        size--;
        return removed;
    }
    void clear() {
        if (arr != nullptr) {
            delete[] arr;
        }
        size = 0;
        cap = 0;
    }
};

class Line {
private:
    Vector<char> text;
public:
    size_t position;
    bool selected;

    char* getArr() const {
        return text.getArr();
    }

    size_t getSize() const {
        return text.getSize();
    }

    size_t getCap() {
        return text.getCap();
    }

    Line() {
        position = 0;
        selected = false;
    }

    char* getText() const {
        return text.getArr();
    }

    void insertChar(char c) {
        text.insert(position, c);
        position++;
    }

    char removeChar() {
        if (position == 0) return '\0';
        char deleted = text.erase(position - 1);
        position--;
        return deleted;
    }

    void cutOff() {
        text.setSize(position);
    }

};

class KeyAction {
private:
    Vector<int> keyCodes;
    void(*handler)(int code);
public:
    KeyAction(void(*_handler)(int code)) :
        handler(_handler)
    {
    }
    KeyAction(Vector<int> _keyCodes, void(*_handler)(int code)) :
        keyCodes(_keyCodes),
        handler(_handler)
    {
    }
    KeyAction(int _keyCode, void(*_handler)(int code)) :
        handler(_handler)
    {
        keyCodes.push(_keyCode);
    }

    void setCodes(Vector<int> _keyCodes) {
        keyCodes = _keyCodes;
    }
    void setCode(int keyCode) {
        keyCodes.clear();
        keyCodes.push(keyCode);
    }

    void handle(int code) {
        handler(code);
    }
};

class File {
public:
    Vector<Line*> lines;
    size_t currentLine;

    File() {
        currentLine = 0;
        lines.push(new Line());
        auto& start = lines.getArr()[0];
        start->position = 0;
    }


    Line* getCurrentLine() {
        return lines.getArr()[currentLine];
    }

    void insertChar(int c) {
        auto* selectedLine = getCurrentLine();
        selectedLine->insertChar(c);
    }

    char removeChar() {
        auto* selectedLine = getCurrentLine();
        if (selectedLine->position == 0) {
            if (lines.getSize() == 1) {
                return '\0';
            }
            delete lines.getArr()[currentLine];
            lines.erase(currentLine);
            if (currentLine > 0)
                currentLine--;
            selectedLine = getCurrentLine();
            selectedLine->position = selectedLine->getSize();

            return '\0';
        }
        return selectedLine->removeChar();
    }

    void newLineAt() {
        auto* selectedLine = getCurrentLine();
        Line* newLine = new Line();
        for (size_t i = selectedLine->position; i < selectedLine->getSize(); i++) {
            newLine->insertChar(selectedLine->getArr()[i]);
        }
        selectedLine->cutOff();
        lines.insert(currentLine + 1, newLine);
        newLine->position = 0;
        currentLine++;
    }

    void cursorDown() {
        if (currentLine + 1 < lines.getSize()) {
            auto* selectedLine = getCurrentLine();
            auto* newLine = lines.getArr()[currentLine + 1];
            if (selectedLine->position > newLine->getSize()) {
                newLine->position = newLine->getSize();
            }
            else {
                newLine->position = selectedLine->position;
            }
            currentLine++;
        }
    }

    void cursorUp() {
        if (currentLine > 0) {
            auto* selectedLine = getCurrentLine();
            auto* newLine = lines.getArr()[currentLine - 1];
            if (selectedLine->position == 0 || selectedLine->position > newLine->getSize()) {
                newLine->position = newLine->getSize();
            }
            else {
                newLine->position = selectedLine->position;
            }
            currentLine--;
        }
    }

    void cursorRight() {
        auto* selectedLine = getCurrentLine();
        if (selectedLine->position == selectedLine->getSize()) {
            if (currentLine + 1 < lines.getSize()) {
                auto* newLine = lines.getArr()[currentLine + 1];
                newLine->position = 0;
                currentLine++;
            }
        }
        else {
            selectedLine->position++;
        }
    }

    void cursorLeft() {
        auto* selectedLine = getCurrentLine();
        if (selectedLine->position == 0) {
            if (currentLine > 0) {
                auto* newLine = lines.getArr()[currentLine - 1];
                newLine->position = newLine->getSize();
            }
        }
        else {
            selectedLine->position--;
        }
    }
};

void leftHandler() {
    Global::file.cursorLeft();
}

void rightHandler() {
    Global::file.cursorRight();
}

void upHandler() {
    Global::file.cursorUp();
}

void downHandler() {
    Global::file.cursorDown();
}

void arrows(int code) {
    switch (code) {
    case KEY_LEFT:
        leftHandler();
        break;
    case KEY_RIGHT:
        rightHandler();
        break;
    case KEY_UP:
        upHandler();
        break;
    case KEY_DOWN:
        downHandler();
        break;
    }
}

void handleEnter(int code) {
    Global::file.newLineAt();
}

void handleEscape(int code) {
    Global::running = false;
}

void handleBackspace(int code) {
    Global::file.removeChar();
}

void handleHome(int code) {
    Global::file.getCurrentLine()->position = 0;
}

void handleEnd(int code) {
    Global::file.getCurrentLine()->position = Global::file.getCurrentLine()->getSize();
}

std::map<Vector<int>, KeyAction> Global::specialKeys = {
    {{KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN},KeyAction(arrows)},
    {{KEY_ENTER, 10},KeyAction(handleEnter)},
    {{27},KeyAction(handleEscape)},
    {{8, KEY_BACKSPACE},KeyAction(handleBackspace)},
    {{KEY_HOME},KeyAction(handleHome)},
    {{KEY_END},KeyAction(handleEnd)},
};

std::unordered_map<int, KeyAction*> Global::keyLookup;

void installSpecialKeys() {
    for (auto& [codes, action] : Global::specialKeys) {
        for (size_t i = 0; i < codes.getSize(); i++) {
            Global::keyLookup[codes.getArr()[i]] = &action;
        }
    }
}

File Global::file;

size_t getCursorX() {
    Global::cursorX = Global::file.lines.getArr()[Global::file.currentLine]->position;
    return Global::cursorX;
}

size_t getCursorY() {
    Global::cursorY = Global::file.currentLine;
    return Global::cursorY;
}

size_t Global::cursorX = 0;
size_t Global::cursorY = 0;

int main() {
    initscr();
    start_color();
    clear();
    noecho();
    cbreak();	/* Line buffering disabled. pass on everything */
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    installSpecialKeys();
    while (Global::running) {
        int c = getch();
        if (Global::keyLookup.find(c) != Global::keyLookup.end()) {
            Global::keyLookup.at(c)->handle(c);
        }
        else {
            Global::file.insertChar(c);
        }

        size_t cx = getCursorX();
        size_t cy = getCursorY();

        for (size_t i = 0; i < Global::file.lines.getSize(); i++) {
            Line* line = Global::file.lines.getArr()[i];
            move(i, 0);
            clrtoeol();
            for (size_t j = 0; j < line->getSize(); j++) {
                if (cx == j && cy == i) {
                    attron(COLOR_PAIR(1));
                    addch(line->getArr()[j]);
                    attroff(COLOR_PAIR(1));
                }
                else {
                    addch(line->getArr()[j]);
                }
            }

            // special case: cursor at "end of line"
            if (cy == i && cx == line->getSize()) {
                attron(COLOR_PAIR(1));
                mvaddch(i, cx, ' ');
                attroff(COLOR_PAIR(1));
            }
        }

        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);  // terminal dimensions

        // clear everything after the last line in buffer
        for (size_t i = Global::file.lines.getSize(); i < (size_t)maxY; i++) {
            move(i, 0);
            clrtoeol();
        }

        // move ncurses cursor visually to match
        move(cy, cx);

        refresh();
    }

    endwin();
}