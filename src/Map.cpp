#include "Map.h"

#include "io/File.h"
#include "ecs/Tile.h"
#include "ecs/Transform.h"
#include "ecs/Animation.h"
#include "ecs/Collider.h"
#include "ecs/filters/Twilight.h"
#include "ecs/Analytics.h"

// Derive the Asset from the tileset source path recorded in the map JSON.
// source is a relative path like "../TILESETS/Tiles.json"; we match by basename.
static Asset assetFromTilesetSource(const std::string &source) {
    auto slash = source.rfind('/');
    std::string basename = (slash == std::string::npos) ? source : source.substr(slash + 1);
    if (basename == "Tiles.json")   return TILES_BEACH;
    if (basename == "dungeon.json") return TILES_DUNGEON;
    return UNDEFINED;
}

static LayerType layerTypeFromName(const std::string &name) {
    if (name == "background") return BACKGROUND;
    if (name == "floor")      return FLOOR;
    if (name == "walls")      return WALLS;
    if (name == "roof")       return ROOF;
    if (name == "sky")        return SKY;
    if (name == "items")      return ITEMS;
    if (name == "objects")    return OBJECTS;
    if (name == "foreground") return FOREGROUND;
    if (name == "ui")         return UI;
    if (name == "parallax")   return PARALLAX;
    return BACKGROUND;
}

// Find a named property value in a Tiled properties array.
static JSON::Value findProperty(JSON::Value &props, const char *name) {
    if (!props.is(JSON::JSON_ARRAY)) return JSON::null;
    for (auto &prop: props.as<JSON::Array>()) {
        auto propName = prop["name"];
        if (propName.is(JSON::JSON_STRING) && propName.as<std::string>() == name) {
            return prop["value"];
        }
    }
    return JSON::null;
}

// Parse a comma-separated string of ints into a vector.
static void parseCSVInts(const std::string &s, std::vector<int> &out) {
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ','))
        out.push_back(std::stoi(token));
}

TilesetIndex loadTilesetIndex(const std::string &path) {
    TilesetIndex index;

    File f(path);
    if (!f.open()) return index;

    std::string json;
    json.resize(f.size);
    f.read(json, 0, f.size);

    JSON::Parser p;
    JSON::Value v;
    p.parse(v, json);

    auto tiles = v["tiles"];
    if (!tiles.is(JSON::JSON_ARRAY)) return index;

    for (auto &tile: tiles.as<JSON::Array>()) {
        int id = tile["id"].as<int>();
        auto props = tile["properties"];

        TileData data;

        // Native Tiled animation: [{"duration": 500, "tileid": 35}, ...]
        auto nativeAnim = tile["animation"];
        if (nativeAnim.is(JSON::JSON_ARRAY)) {
            auto frames = nativeAnim.as<JSON::Array>();
            if (!frames.empty()) {
                data.speed = frames[0]["duration"].as<int>();
                for (auto &frame: frames)
                    data.frames.push_back(frame["tileid"].as<int>());
            }
        }

        // Interact animation: CSV string "20,21,22"
        auto interactProp = findProperty(props, "interactAnimation");
        if (interactProp.is(JSON::JSON_STRING))
            parseCSVInts(interactProp.as<std::string>(), data.interactFrames);

        auto interactSpeedProp = findProperty(props, "interactSpeed");
        if (interactSpeedProp.is(JSON::JSON_NUMBER))
            data.interactSpeed = interactSpeedProp.as<int>();

        index[id] = data;
    }

    return index;
}

Layer::Layer(const char *baseDirTilesets) : w(0), h(0),
                                            baseDirTilesets(baseDirTilesets) {}

std::shared_ptr<Entity> Layer::getTile(int x, int y) {
    int pos = (y * w) + x;
    return tiles[pos];
}

void Layer::load(JSON::Value &layer, Asset asset) {
    w = layer["width"].as<int>();
    h = layer["height"].as<int>();

    type = layerTypeFromName(layer["name"].as<std::string>());

    auto metadata = getProperty(layer, "metadata");
    if (!metadata.is(JSON::JSON_NULL)) {
        std::string path = baseDirTilesets + "/" + metadata.as<std::string>();
        tilesetIndex = loadTilesetIndex(path);
    }

    auto data = layer["data"].as<JSON::Array>();

    for (size_t i = 0; i < data.size(); ++i) {
        int x, y;
        toPos(w, (int)i, &x, &y);

        // -1 because of the way tiled reserves id 0
        int tileId = data[i].as<int>() - 1;

        // Not all tiles have to be set on a map, skip the empty ones
        if (tileId < 0) {
            tiles.push_back(nullptr);
            continue;
        }

        auto entity = Manager::instance().addEntity(type);
        entity->addComponent<Transform>(x, y);
        entity->addComponent<Tile>(asset);
        entity->addComponent<Animation>(100.0, true);
        entity->addComponent<Analytics>();

        auto anim = entity->getComponent<Animation>();

        auto it = tilesetIndex.find(tileId);
        if (it != tilesetIndex.end()) {
            const TileData &td = it->second;

            if (!td.frames.empty()) {
                // Native animation defines the complete frame sequence
                anim->speed = td.speed;
                for (int frame: td.frames)
                    anim->addAnimationFrame(frame);
            } else {
                anim->addAnimationFrame(tileId);
            }

            for (int frame: td.interactFrames)
                anim->addStateFrame(frame, td.interactSpeed);

            if (type == WALLS) {
                auto transform = entity->getComponent<Transform>();
                entity->addComponent<Collider>(transform, CT_WALL);
            }
        } else {
            anim->addAnimationFrame(tileId);
            if (type == WALLS) {
                auto transform = entity->getComponent<Transform>();
                entity->addComponent<Collider>(transform, CT_WALL);
            }
        }

        tiles.push_back(entity);
    }
}

JSON::Value Layer::getProperty(JSON::Value &layer, const char *prop) {
    auto &propsVal = layer["properties"];
    if (!propsVal.is(JSON::JSON_ARRAY)) {
        return JSON::null;
    }
    auto properties = propsVal.as<JSON::Array>();
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

Map::Map(const char *baseDir, const char *baseDirTilesets) : baseDir(baseDir), baseDirTilesets(baseDirTilesets) {}

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

    Asset asset = UNDEFINED;
    auto tilesets = v["tilesets"].as<Array>();
    if (!tilesets.empty()) {
        auto source = tilesets[0]["source"].as<std::string>();
        asset = assetFromTilesetSource(source);
    }

    auto ll = v["layers"].as<Array>();
    for (auto &layer: ll) {
        auto l = std::make_shared<Layer>(this->baseDirTilesets.c_str());
        l->load(layer, asset);
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
