#include "LevelScreen.h"
#include <iostream>
#include <string>
#include <vector>
#include "../config.h.in"
#include "../game/Walls.h"
#include "../game/Spawner/Level1Spawner.h"
#include "../game/interactables/PushBlock.h"

// Konstruktor ist identisch
LevelScreen::LevelScreen(int *level_Ptr) : Level_Nbr_Ptr(level_Ptr) {
    this->loaded = false;
}

// Destruktor ist identisch
LevelScreen::~LevelScreen() {
    if (tileatlas_Texture.id != 0) {
        UnloadTexture(tileatlas_Texture);
    }
}

// Load_Levelmap ist identisch
void LevelScreen::Load_Levelmap() {
    int Level = *Level_Nbr_Ptr;
    tson::Tileson parser;
    std::string levelmap_Path = game::Config::GetLevelMapPath(Level);

    map = parser.parse(levelmap_Path);

    if (map == nullptr || map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "FEHLER: Konnte die Map nicht laden oder parsen: " << levelmap_Path << std::endl;
        std::cerr << "Status: " << map->getStatusMessage() << "\n";
        return;
    }

    for (auto &tileset: map->getTilesets()) {
        std::string image_Path_Raw = tileset.getImagePath().string();
        if (image_Path_Raw.substr(0, 3) == "../") {
            image_Path_Raw = image_Path_Raw.substr(3);
        }
        std::string image_Path = "../../assets/Tiled/" + image_Path_Raw;
        tileatlas_Texture = LoadTexture(image_Path.c_str());
        if (tileatlas_Texture.id == 0) {
            std::cerr << "FEHLER: Konnte das Tileset nicht laden: " << image_Path << std::endl;
        }
        break;
    }
}

void LevelScreen::Draw_Level(std::shared_ptr<Cam> kamera, bool aboveObjects) {
    if (map == nullptr) {
        return;
    }

    // Die Funktion kümmert sich NUR noch um das Zeichnen der Kacheln.
    for (auto &layer: map->getLayers()) {
        if (!layer.isVisible() || layer.getType() != tson::LayerType::TileLayer) {
            continue;
        }
        bool isAbove = false;
        if (layer.getProperties().hasProperty("IsAboveObjects")) {
            auto *prop = layer.getProperties().getProperty("IsAboveObjects");
            if (prop != nullptr && prop->getType() == tson::Type::Boolean) {
                isAbove = prop->getValue<bool>();
            }
        }
        if (isAbove != aboveObjects)
            continue;

        auto &tile_Layer = layer.getTileData();
        for (const auto &pair: tile_Layer) {
            int x = std::get<0>(pair.first);
            int y = std::get<1>(pair.first);
            tson::Tile *tile = pair.second;
            if (tile != nullptr) {
                tson::Rect drawingRect = tile->getDrawingRect();
                tson::Vector2f worldPos = { (float)(x * map->getTileSize().x), (float)(y * map->getTileSize().y) };
                Rectangle srcRect = { (float)drawingRect.x, (float)drawingRect.y, (float)drawingRect.width, (float)drawingRect.height };
                Vector2 destPos = { (float)worldPos.x, (float)worldPos.y };
                DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
            }
        }
    }
}

void LevelScreen::LoadGameObjects(Object_Manager& g_objectManager) {
    if (!this->loaded){
        Load_Levelmap();
        this->loaded = true;
    }
    if (map == nullptr) { return; }

    static std::vector<Rectangle> temp_obstacle_list;
    static std::vector<enemy::Enemy_Base_Class*> temp_raw_enemy_list;

    for (auto &layer: map->getLayers()) {
        if (layer.getType() == tson::LayerType::ObjectGroup) {

            for (auto &object: layer.getObjects()) {

                std::shared_ptr<Collidable> new_object = nullptr;
                const std::string& object_name = object.getName();

                if (object_name == "walls") {
                    Vector2 temp_pos = { (float)object.getPosition().x, (float)object.getPosition().y };
                    Vector2 temp_size = { (float)object.getSize().x, (float)object.getSize().y };
                    new_object = std::make_shared<Walls>(temp_pos, temp_size);
                }
                else if (object_name == "movWall") {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }
                        if (tile) {
                            Vector2 pos = { (float)object.getPosition().x, (float)object.getPosition().y };
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<Push_Block>(pos, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "spawn1") {
                    Rectangle spawner_area = { (float)object.getPosition().x, (float)object.getPosition().y, (float)object.getSize().x, (float)object.getSize().y };
                    float spawn_rate = game::Config::kSpawnRateSpawn1;
                    int max_enemies = game::Config::kMaxEnemiesSpawn1;
                    if(object.getProperties().hasProperty("spawn_rate")) spawn_rate = object.getProperties().getValue<float>("spawn_rate");
                    if(object.getProperties().hasProperty("max_enemies")) max_enemies = object.getProperties().getValue<int>("max_enemies");
                    new_object = std::shared_ptr<Level1_Spawner>(new Level1_Spawner(spawner_area, temp_obstacle_list, temp_raw_enemy_list, spawn_rate, max_enemies, g_objectManager));
                }
                else if (object_name == "player_start") {
                    std::cout << "Spieler-Startpunkt gefunden bei: " << object.getPosition().x << ", " << object.getPosition().y << std::endl;
                }

                if (new_object != nullptr) {
                    if (object.getProperties().hasProperty("useFog")) {
                        new_object->Set_Use_Fog(object.getProperties().getValue<bool>("useFog"));
                    }
                    g_objectManager.AddObject(new_object);
                }
            }
        }
    }
}
