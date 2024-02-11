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

class Layer {
public:
    Layer();

    void load(JSON::Value &layer);

    std::shared_ptr<Entity> getTile(int x, int y);

    void topology(std::vector<bool> &t);

    int w;
    int h;
    LayerType type;
    Asset asset;

private:

    void toPos(int w, int n, int *x, int *y);

    JSON::Value getProperty(JSON::Value &layer, const char *prop);

    std::vector<std::shared_ptr<Entity>> tiles;
};

class Map {
public:

    Map(const char *baseDir);

    bool load(const char *file);

    void topology(std::vector<bool> &t);

    std::shared_ptr<Entity> getTile(LayerType layer, int x, int y);

    void getSize(LayerType layer, int *w, int *h);

    void clear();

private:
    std::string baseDir;

    std::unordered_map<LayerType, std::shared_ptr<Layer>> layers;
};


#endif
