#include "Map.h"

#include "io/File.h"
#include "ecs/Tile.h"
#include "ecs/Transform.h"
#include "ecs/Animation.h"
#include "ecs/Collider.h"
#include "ecs/filters/Twilight.h"

void Tileset::load(const char *file) {
    using namespace JSON;

    File f(file);
    f.open();

    std::string json;
    json.resize(f.size);
    f.read(json, 0, f.size);
    p.parse(v, json);
}

JSON::Value Tileset::getProperty(int tileId, const char *name) {
    auto props = getPropsForTile(tileId);

    if (!props.is(JSON::JSON_ARRAY)) {
        return JSON::null;
    }

    for (auto &prop : props.as<JSON::Array>()) {
        auto propName = prop["name"];
        if (!propName.is(JSON::JSON_STRING)) {
            continue;
        }

        if (propName.as<std::string>().compare(name) == 0) {
            return prop["value"];
        }
    }
    return JSON::null;
}

bool Tileset::getFrames(int tileId, std::vector<int> &frames) {
    auto prop = getProperty(tileId, "animation");
    if (!prop.is(JSON::JSON_STRING)) {
        return false;
    }

    JSON::Value f;
    p.parse(f, prop.as<std::string>());
    for (auto &frame : f.as<JSON::Array>()) {
        frames.push_back(frame.as<int>());
    }

    return true;
}

bool Tileset::getSpeed(int tileId, int *speed) {
    auto s = getProperty(tileId, "speed");
    if (!s.is(JSON::JSON_NUMBER)) {
        return false;
    }
    *speed = s.as<int>();
    return true;
}

JSON::Value Tileset::getPropsForTile(int tileId) {
    auto tiles = v["tiles"];
    if (!tiles.is(JSON::JSON_ARRAY)) {
        return JSON::null;
    }

    for (auto &tile : tiles.as<JSON::Array>()) {
        auto id = tile["id"].as<int>();
        if (id == tileId) {
            return tile["properties"];
        }
    }
    return JSON::null;
}

bool Tileset::hasProps(int tileId) {
    return getPropsForTile(tileId).is(JSON::JSON_NULL) == false;
}

Layer::Layer(const char *baseDir) : w(0), h(0), baseDir(baseDir) {}

std::shared_ptr<Entity> Layer::getTile(int x, int y) {
    int pos = (y * w) + x;
    return tiles[pos];
}

void Layer::load(JSON::Value &layer) {
    w = layer["width"].as<int>();
    h = layer["height"].as<int>();

    type = static_cast<LayerType>(getProperty(layer, "layerType").as<int>());
    asset = static_cast<Asset>(getProperty(layer, "assetId").as<int>());

    auto metadata = getProperty(layer, "metadata");
    if (!metadata.is(JSON::JSON_NULL)) {
        tileset = std::make_shared<Tileset>();

        std::stringstream ss;
        ss << baseDir << "/" << metadata.as<std::string>();
        tileset->load(ss.str().c_str());
    }

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
        entity->addComponent<Tile>(asset);
        entity->addComponent<Animation>(100.0, true);
        entity->getComponent<Animation>()->addAnimationFrame(tileId);

        if (type == WALLS) {
            auto transform = entity->getComponent<Transform>();
            entity->addComponent<Collider>(transform, CT_WALL);
        }

        if (tileset && tileset->hasProps(tileId)) {
            int speed;
            std::vector<int> frames;

            if (tileset->getFrames(tileId, frames)) {
                for (int frame : frames) {
                    entity->getComponent<Animation>()->addAnimationFrame(frame);
                }
            }
            if (tileset->getSpeed(tileId, &speed)) {
                entity->getComponent<Animation>()->speed = speed;
            }
        }

        tiles.push_back(entity);
    }
}

JSON::Value &Layer::getProperty(JSON::Value &layer, const char *prop) {
    auto properties = layer["properties"].as<JSON::Array>();
    for (auto &property : properties) {
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

Map::Map(const char *baseDir) : baseDir(baseDir) {}

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
    for (auto &layer : ll) {
        auto l = std::make_shared<Layer>(this->baseDir.c_str());
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