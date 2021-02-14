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

# Building on macos

Install dependencies

```
brew install SDL2
brew install SDL2_image
brew install SDL2_mixer
brew install cmake
```

Build it

```
cmake .
cmake --build .