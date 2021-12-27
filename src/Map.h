#ifndef ZWEI_MAP_H
#define ZWEI_MAP_H

#include <vector>
#include <unordered_map>
#include <sstream>

#include <JSON/parser.h>
#include <ASSETS/Assets.h>

#include "ecs/Manager.h"
#include "io/File.h"
#include "alg/Padding.h"

class Tileset {
public:
    void load(const char *file);

    bool getFrames(int tileId, const char *key, std::vector<int> &frames);

    bool getInt(int tileId, const char *key, int *speed);

    bool getPadding(int tileId, Padding &p);

    bool hasProps(int tileId);

private:
    JSON::Value getProperty(int tileId, const char *prop);

    JSON::Value getPropsForTile(int tileId);

    JSON::Value v;
    JSON::Parser p;
};

class Layer {
public:
    Layer(const char *baseDirTilesets);

    void load(JSON::Value &layer);

    std::shared_ptr<Entity> getTile(int x, int y);

    void topology(std::vector<bool> &t);

    int w;
    int h;
    LayerType type;
    Asset asset;

private:

    void toPos(int w, int n, int *x, int *y);

    std::string baseDirTilesets;

    JSON::Value getProperty(JSON::Value &layer, const char *prop);

    std::vector<std::shared_ptr<Entity>> tiles;

    std::shared_ptr<Tileset> tileset;
};

class Map {
public:

    Map(const char *baseDir, const char *baseDirTilesets);

    bool load(const char *file);

    void topology(std::vector<bool> &t);

    std::shared_ptr<Entity> getTile(LayerType layer, int x, int y);

    void getSize(LayerType layer, int *w, int *h);

    void clear();

private:
    std::string baseDir;

    std::string baseDirTilesets;

    std::unordered_map<LayerType, std::shared_ptr<Layer>> layers;
};


#endif
