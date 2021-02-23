# Zwei

Game engine for 2d action RPG style games. Work in progress, still very rough.

- [x] Layers, Sprites, Animations
- [x] Import Maps and Tilesets from [Tiled](https://www.mapeditor.org/)
- [x] Items, Weapons and Inventory
- [x] Combat
- [x] Menu
- [ ] Save & Load 
- [ ] Scripting
- [ ] Building on Windows and Mac OS 
- [ ] Docs

# Screenshots

![Gameplay](./media/screenshot.png)

![Menu](./media/menu.png)

# Building

## macos

Install dependencies

```
brew install cmake sdl2 sdl_image sdl_mixer
```

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
