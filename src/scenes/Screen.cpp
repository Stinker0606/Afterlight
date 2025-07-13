#include "Screen.h"
#include <iostream>
#include "../config.h.in"

Screen::Screen(int *level_Ptr) : Level_Nbr_Ptr(level_Ptr), loaded(false) {}

Screen::~Screen() {
    if (tileatlas_Texture.id != 0) UnloadTexture(tileatlas_Texture);
}

void Screen::Load_Levelmap() {
    if (loaded) return;
    int Level = *Level_Nbr_Ptr;
    tson::Tileson parser;
    std::string levelmap_Path = game::Config::GetLevelMapPath(Level);
    map = parser.parse(levelmap_Path);
    if (map == nullptr || map->getStatus() != tson::ParseStatus::OK) return;

    for (auto &tileset: map->getTilesets()) {
        std::string image_Path_Raw = tileset.getImagePath().string();
        if (image_Path_Raw.substr(0, 3) == "../") image_Path_Raw = image_Path_Raw.substr(3);
        std::string image_Path = "../../assets/Tiled/" + image_Path_Raw;
        tileatlas_Texture = LoadTexture(image_Path.c_str());
        break;
    }
    loaded = true;
}

void Screen::UpdateFog(Vector2 playerPosition, float deltaTime) {
    fogManager.Update(playerPosition, deltaTime);
}

void Screen::Draw_Level(std::shared_ptr<Cam> kamera, bool aboveObjects) {
    if (map == nullptr) return;
    for (auto &layer: map->getLayers()) {
        if (!layer.isVisible() || layer.getType() != tson::LayerType::TileLayer) continue;
        bool isAbove = false;
        if (layer.getProperties().hasProperty("IsAboveObjects")) {
            auto *prop = layer.getProperties().getProperty("IsAboveObjects");
            if (prop != nullptr && prop->getType() == tson::Type::Boolean) {
                isAbove = prop->getValue<bool>();
            }
        }
        if (isAbove != aboveObjects) continue;
        auto &tile_Layer = layer.getTileData();
        for (const auto &pair: tile_Layer) {
            int x = std::get<0>(pair.first);
            int y = std::get<1>(pair.first);
            tson::Tile *tile = pair.second;
            if (tile != nullptr) {
                tson::Rect drawingRect = tile->getDrawingRect();
                Rectangle srcRect = { (float)drawingRect.x, (float)drawingRect.y, (float)drawingRect.width, (float)drawingRect.height };
                Vector2 destPos = { (float)(x * map->getTileSize().x), (float)(y * map->getTileSize().y) };
                DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
            }
        }
    }
}

void Screen::LoadGameObjects(Object_Manager& g_objectManager) {
    if (!loaded) Load_Levelmap();
    if (map == nullptr) return;
    for (auto &layer: map->getLayers()) {
        if (layer.getType() == tson::LayerType::ObjectGroup) {
            for (auto &object: layer.getObjects()) {
                if (layer.getName() == "walls") {
                    g_objectManager.AddObject(new Walls({(float)object.getPosition().x, (float)object.getPosition().y}, {(float)object.getSize().x, (float)object.getSize().y}));
                }
            }
        }
    }
}