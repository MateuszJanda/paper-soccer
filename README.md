## paper-soccer
A networked version of paper soccer game in modern console.

<p align="center">
<img src="./gallery/screenshot.png" alt="paper-soccer"/>
</p>

## Requirements
Tested on:
- `gcc` with `C++20` support
- `googletest` (1.12.1)
- `protobuf (libprotobuf-dev)` (3.12.4)
- `protoc` (protobuf-compiler) (3.13.4) - command line tool
- `boost (libboost-all-dev)` (1.74)
    - `boost.asio`
    - `boost.program_options`
- `ncurses` (6.3)
- also terminal supporting font fallback ("weird" Unicode characters are used)

## Installation
```bash
git clone https://github.com/MateuszJanda/paper-soccer --recursive

cd paper-soccer
cmake CMakeLists.txt
make

# To run tests
make runTests
./runTests
```

## Example
Run as server and wait for connection (default port `8787`)
```bash
./paper-soccer --wait

# Equivalent
./paper-soccer --wait --port 8787
```

Connect to localhost server
```bash
./paper-soccer --connect localhost

# Equivalent
./paper-soccer --connect localhost --port 8787
```

### Game control
Key | Direction
--- | ---
<kbd>Enter</kbd> | End trun
<kbd>u</kbd> | Undo player move
<kbd>n</kbd> | Start new game, when this game end
<kbd>Ctrl+c</kbd> | Exit

### Ball movements
Ball placement can be changed by mouse or keyboard.

Key | Direction
--- | ---
<kbd>w</kbd> | Top
<kbd>x</kbd> | Bottom
<kbd>a</kbd> | Left
<kbd>d</kbd> | Right
<kbd>q</kbd> | Top-Left
<kbd>e</kbd> | Top-Right
<kbd>z</kbd> | Bottom-Left
<kbd>c</kbd> | Bottom-Right

## Code style settings
```
clang-format -style='{BasedOnStyle: WebKit, Cpp11BracedListStyle: true, SpaceBeforeCpp11BracedList: false}' -i *.?pp
```