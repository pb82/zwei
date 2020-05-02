#ifndef ZWEI_MAP_H
#define ZWEI_MAP_H

#include <vector>
#include <unordered_map>

#include <JSON/parser.h>
#include <ASSETS/Assets.h>

#include "ecs/Manager.h"
#include "ecs/Layered.h"
#include "io/File.h"

class Layer {
public:
    Layer() : w(0), h(0) {}

    void load(JSON::Value &layer);

    std::shared_ptr<Entity> getTile(int x, int y);

    int w;
    int h;
    LayerType type;
    Asset asset;

private:

    void toPos(int w, int n, int *x, int *y);

    JSON::Value &getProperty(JSON::Value &layer, const char *prop);

    std::vector<std::shared_ptr<Entity>> tiles;
};

class Map {
public:

    bool load(const char *file);

    std::shared_ptr<Entity> getTile(LayerType layer, int x, int y);

private:

    std::unordered_map<LayerType, std::shared_ptr<Layer>> layers;
};


#endif
