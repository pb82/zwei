# Zwei

Game engine for 2d action RPG style games. Work in progress, still very rough.

- [x] Layers, Sprites, Animations
- [x] Import Maps and Tilesets from [Tiled](https://www.mapeditor.org/)
- [x] Items, Weapons and Inventory
- [x] Combat
- [x] Menu
- [x] Builds on Linux and MacOs
- [ ] Save & Load
- [ ] Scripting
- [ ] Building on Windows
- [ ] Docs

# Screenshots

[![Gameplay Video](https://img.youtube.com/vi/rK77goqBpAo/0.jpg)](https://www.youtube.com/watch?v=rK77goqBpAo)

![Gameplay](./media/screenshot.png)

![Menu](./media/menu.png)

# Building

## macos

Install dependencies

```
brew install cmake
```

__NOTE__: All other dependencies are included in the repository

Build using cmake

```
cmake .
cmake --build .
```

Run it

```
./zwei
```

To clean and rebuild

```
rm CMakeCache.txt && rm -rf CMakeFiles/ && rm Makefile && rm cmake_install.cmake  && rm zwei
cmake .
cmake --build .
```
