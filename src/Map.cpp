#include "Map.h"

#include "io/File.h"
#include "ecs/Tile.h"
#include "ecs/Transform.h"
#include "ecs/Animation.h"

std::shared_ptr<Entity> Layer::getTile(int x, int y) {
    int pos = (y * w) + x;
    return tiles[pos];
}

void Layer::load(JSON::Value &layer) {
    w = layer["width"].as<int>();
    h = layer["height"].as<int>();

    type = static_cast<LayerType>(getProperty(layer, "layerType").as<int>());
    asset = static_cast<Asset>(getProperty(layer, "assetId").as<int>());

    auto data = layer["data"].as<JSON::Array>();
    for (int i = 0; i < data.size(); ++i) {
        int x, y;
        toPos(w, i, &x, &y);

        int tileId = data[i].as<int>();

        auto entity = Manager::instance().addEntity();
        entity->addComponent<Transform>(x, y);
        entity->addComponent<Tile>(asset);
        entity->addComponent<Layered>(type);
        entity->addComponent<Animation>(100.0, false);
        entity->getComponent<Animation>()->addAnimationFrame(tileId);
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

std::shared_ptr<Entity> Map::getTile(LayerType layer, int x, int y) {
    return layers[layer]->getTile(x, y);
}

bool Map::load(const char *file) {
    using namespace JSON;

    File f(file);
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
        auto l = std::make_shared<Layer>();
        l->load(layer);
        layers.emplace(l->type, l);
    }

    return true;
}
