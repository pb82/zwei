#include "Map.h"

#include "io/File.h"
#include "ecs/Tile.h"
#include "ecs/Transform.h"
#include "ecs/Animation.h"
#include "ecs/Collider.h"
#include "ecs/filters/Twilight.h"
#include "ecs/Analytics.h"

Layer::Layer() : w(0), h(0) {}

std::shared_ptr<Entity> Layer::getTile(int x, int y) {
    int pos = (y * w) + x;
    return tiles[pos];
}

void Layer::load(JSON::Value &layer) {
    w = layer["width"].as<int>();
    h = layer["height"].as<int>();

    type = static_cast<LayerType>(getProperty(layer, "layerType").as<int>());


    auto data = layer["data"].as<JSON::Array>();

    for (int i = 0; i < data.size(); ++i) {
        int x, y;
        toPos(w, i, &x, &y);

        // -1 because of the way tiled reserves id 0
        int tileId = data[i].as<int>() - 1;

        // Not all tiles have to be set on a map, skip the
        // empty ones
        if (tileId < 0) {
            tiles.push_back(nullptr);
            continue;
        }

        auto entity = Manager::instance().addEntity(type);
        entity->addComponent<Transform>(x, y);
        entity->addComponent<Tile>(TILES_BEACH);
        entity->addComponent<Animation>(100.0, true);
        entity->getComponent<Animation>()->addAnimationFrame(tileId);
        entity->addComponent<Analytics>();

        if (type == WALLS) {
            auto transform = entity->getComponent<Transform>();
            entity->addComponent<Collider>(transform, CT_WALL);
        }

        tiles.push_back(entity);
    }
}

JSON::Value Layer::getProperty(JSON::Value &layer, const char *prop) {
    auto properties = layer["properties"].as<JSON::Array>();
    for (auto &property: properties) {
        auto name = property["name"].as<std::string>();
        if (name.compare(prop) == 0) {
            return property["value"];
        }
    }
    return JSON::null;
}

void Layer::toPos(int w, int n, int *x, int *y) {
    *x = n % w;
    *y = floor(n / w);
}

Map::Map(const char *baseDir) : baseDir(baseDir){}

std::shared_ptr<Entity> Map::getTile(LayerType layer, int x, int y) {
    return layers[layer]->getTile(x, y);
}

void Map::getSize(LayerType layer, int *w, int *h) {
    auto l = layers.find(layer);
    if (l != layers.end()) {
        *w = l->second->w;
        *h = l->second->h;
    }
}

bool Map::load(const char *file) {
    using namespace JSON;

    std::stringstream ss;
    ss << baseDir << "/" << file;

    File f(ss.str());
    bool success = f.open();
    if (!success) {
        return success;
    }

    std::string json;
    json.resize(f.size);
    f.read(json, 0, f.size);

    Value v;
    Parser p;
    p.parse(v, json);

    auto ll = v["layers"].as<Array>();
    for (auto &layer: ll) {
        auto l = std::make_shared<Layer>();
        l->load(layer);
        layers.emplace(l->type, l);
    }

    return true;
}

void Layer::topology(std::vector<bool> &t) {
    int max = this->w * this->h;
    t.resize(max);

    for (int x = 0; x < this->w; x++) {
        for (int y = 0; y < this->h; y++) {
            int pos = (y * w) + x;
            auto tile = this->getTile(x, y);
            if (tile != nullptr) {
                t.at(pos) = true;
            } else {
                t.at(pos) = false;
            }
        }
    }
}

void Map::topology(std::vector<bool> &t) {
    if (layers.find(WALLS) == layers.end()) return;
    auto layer = layers.at(WALLS);
    layer->topology(t);
}

void Map::clear() {
    this->layers.clear();
}