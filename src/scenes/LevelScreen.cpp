#include "LevelScreen.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../config.h.in"
#include "../config_enemies.h.in"
#include "../game/Walls.h"
#include "../game/Spawner/Level1Spawner.h"
#include "../game/spawner/Level2Spawner.h"
#include "../game/Spawner/SpecificSpawner.h"
#include "../game/interactables/interact_list.h"


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

void LevelScreen::LoadSpecificLevelmap(const std::string& map_filename) {
    tson::Tileson parser;
    std::string levelmap_Path = "../../assets/Tiled/Levelmaps/" + map_filename;

    map = parser.parse(levelmap_Path);

    if (map == nullptr || map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "FEHLER: Konnte die Map nicht laden oder parsen: " << levelmap_Path << std::endl;
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

void LevelScreen::Hide_Tiles_In_Area(Rectangle area_to_hide)
{
    if (map == nullptr) return;

    int tile_width = map->getTileSize().x;
    int tile_height = map->getTileSize().y;

    int start_x = floorf(area_to_hide.x / tile_width);
    int start_y = floorf(area_to_hide.y / tile_height);
    int end_x = floorf((area_to_hide.x + area_to_hide.width) / tile_width);
    int end_y = floorf((area_to_hide.y + area_to_hide.height) / tile_height);

    // Füge die Koordinaten aller Kacheln im Bereich zur "schwarzen Liste" hinzu
    for (int y = start_y; y < end_y; ++y) {
        for (int x = start_x; x < end_x; ++x) {
            hidden_tiles_.emplace_back(x, y);
        }
    }
}

void LevelScreen::Draw_Level(std::shared_ptr<Cam> kamera, bool aboveObjects) {
    if (map == nullptr) return;
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
            // Erstelle ein Tupel für die aktuelle Kachel-Koordinate
            std::tuple<int, int> current_tile_coords = {x, y};
            // Prüfe, ob diese Koordinate in unserer "schwarzen Liste" ist.
            if (std::find(hidden_tiles_.begin(), hidden_tiles_.end(), current_tile_coords) != hidden_tiles_.end())
            {
                continue; // Wenn ja, überspringe das Zeichnen dieser Kachel.
            }
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
                else if (object_name == "movWall")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }

                        if (tile) {
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                                };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<Push_Block>(pos, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "keyWall")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }

                        if (tile) {
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<KeyWall>(pos, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "breakWall")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }

                        if (tile) {
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<BreakableWall>(pos, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "doors")
                {
                    // Lese die Custom Properties aus Tiled aus.
                    std::string target_map = "default.json";
                    std::string target_spawn = "player_start";

                    if (object.getProperties().hasProperty("target_map")) {
                        target_map = object.getProperties().getValue<std::string>("target_map");
                    }
                    if (object.getProperties().hasProperty("target_spawn_point")) {
                        target_spawn = object.getProperties().getValue<std::string>("target_spawn_point");
                    }
                    // Erstelle die Hitbox und das Door-Objekt.
                    Rectangle rect = { (float)object.getPosition().x, (float)object.getPosition().y, (float)object.getSize().x, (float)object.getSize().y };
                    new_object = std::make_shared<Door>(rect, target_map, target_spawn);
                }
                // Erkennt den Standard-Startpunkt UND alle benannten Startpunkte
                else if (object_name == "player_start" || object_name.rfind("player_start_", 0) == 0)
                {
                    // Speichere den Namen und die Position des Objekts in unserem "Gedächtnis"
                    spawn_points_[object_name] = { (float)object.getPosition().x, (float)object.getPosition().y };
                }
                else if (object_name.rfind("spawn", 0) == 0) // Erkennt alle Spawner
                {
                    Rectangle spawner_area = { (float)object.getPosition().x, (float)object.getPosition().y, (float)object.getSize().x, (float)object.getSize().y };

                    if (object_name.find('_') != std::string::npos)
                    {
                        // --- FALL 1: SPEZIFISCHER SPAWNER (z.B. "spawn_sniper") ---
                        std::string enemy_name = object_name.substr(6);
                        enemy::EnemyType type;
                        bool found = true;

                        // Standardwerte aus der Config laden
                        float spawn_rate = game::EnemyConfig::kSpecificSpawner_SpawnRate;
                        int max_enemies = game::EnemyConfig::kSpecificSpawner_MaxEnemies;

                        // Prüfe, welcher Gegner es ist und wende die spezifische Ausnahme an
                        if (enemy_name == "sniper") {
                            type = enemy::EnemyType::DROWNED_SNIPER;
                            max_enemies = game::EnemyConfig::kDrownedSniper_MaxSpawnCount;
                        } else if (enemy_name == "sniper2") {
                            type = enemy::EnemyType::WOOD_SNIPER;
                            max_enemies = game::EnemyConfig::kWoodSniper_MaxSpawnCount;
                        } else if (enemy_name == "insect") {
                            type = enemy::EnemyType::INSECT_MONSTER;
                            max_enemies = game::EnemyConfig::kInsectMonster_MaxSpawnCount;
                        } else if (enemy_name == "Corpse") {
                            type = enemy::EnemyType::WALKING_CORPSE;
                            max_enemies = game::EnemyConfig::kWalkingCorpse_MaxSpawnCount;
                        } else if (enemy_name == "Corpse2") {
                            type = enemy::EnemyType::CORPSE;
                            max_enemies = game::EnemyConfig::kCorpse_MaxSpawnCount;
                        } else {
                            found = false;
                        }

                        if (found) {
                            // Überschreibe die Config-Werte, wenn in Tiled etwas anderes steht
                            if(object.getProperties().hasProperty("spawn_rate")) spawn_rate = object.getProperties().getValue<float>("spawn_rate");
                            if(object.getProperties().hasProperty("max_enemies")) max_enemies = object.getProperties().getValue<int>("max_enemies");

                            new_object = std::make_shared<SpecificSpawner>(spawner_area, spawn_rate, max_enemies, g_objectManager, type);
                        }
                    }
                    else
                    {
                        // --- FALL 2: LEVEL-SPEZIFISCHER POOL-SPAWNER (z.B. "spawn1") ---
                        if (object_name == "spawn1") {
                            // Für Pool-Spawner bleiben die Parameter wie sie waren
                            float spawn_rate = game::EnemyConfig::kSpawner1_SpawnRate;
                            int max_enemies = game::EnemyConfig::kSpawner1_MaxEnemies;
                            if(object.getProperties().hasProperty("spawn_rate")) spawn_rate = object.getProperties().getValue<float>("spawn_rate");
                            if(object.getProperties().hasProperty("max_enemies")) max_enemies = object.getProperties().getValue<int>("max_enemies");

                            new_object = std::make_shared<Level1_Spawner>(spawner_area, temp_obstacle_list, &temp_raw_enemy_list, spawn_rate, max_enemies, g_objectManager);
                        }
                        else if (object_name == "spawn2") {
                            float spawn_rate = game::EnemyConfig::kSpawner2_SpawnRate;
                            int max_enemies = game::EnemyConfig::kSpawner2_MaxEnemies;
                            if(object.getProperties().hasProperty("spawn_rate")) spawn_rate = object.getProperties().getValue<float>("spawn_rate");
                            if(object.getProperties().hasProperty("max_enemies")) max_enemies = object.getProperties().getValue<int>("max_enemies");

                            new_object = std::make_shared<Level2_Spawner>(spawner_area, temp_obstacle_list, &temp_raw_enemy_list, spawn_rate, max_enemies, g_objectManager);
                        }
                    }
                }
                else if (object_name == "player_start") {
                    std::cout << "Spieler-Startpunkt gefunden bei: " << object.getPosition().x << ", " << object.getPosition().y << std::endl;
                }
                else if (object_name == "healCons")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }

                        if (tile) {
                            int heal_amount = game::Config::kItemHealConsDefaultAmount;
                            if(object.getProperties().hasProperty("healAmount")) {
                                heal_amount = object.getProperties().getValue<int>("healAmount");
                            }

                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };

                            new_object = std::make_shared<HealConsumable>(pos, heal_amount, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "dmgCons")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }

                        if (tile) {
                            int damage_amount = game::Config::kItemDmgConsDefaultAmount;
                            if(object.getProperties().hasProperty("damageAmount")) {
                                damage_amount = object.getProperties().getValue<int>("damageAmount");
                            }

                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };

                            new_object = std::make_shared<DmgConsumable>(pos, damage_amount, this->tileatlas_Texture, source_rect);
                        }
                    }
                }
                else if (object_name == "keyCons")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }
                        if (tile) {
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<KeyConsumable>(pos, 1, this->tileatlas_Texture, source_rect); // Fügt standardmäßig 1 Schlüssel hinzu
                        }
                    }
                }
                else if (object_name == "bombCons")
                {
                    if (object.getGid() > 0) {
                        tson::Tile* tile = nullptr;
                        for (auto& tileset : map->getTilesets()) {
                            tile = tileset.getTile(object.getGid());
                            if (tile) break;
                        }
                        if (tile) {
                            tson::Rect drawing_rect = tile->getDrawingRect();
                            Vector2 pos = {
                                (float)object.getPosition().x,
                                (float)object.getPosition().y - (float)drawing_rect.height
                            };

                            Rectangle source_rect = { (float)drawing_rect.x, (float)drawing_rect.y, (float)drawing_rect.width, (float)drawing_rect.height };
                            new_object = std::make_shared<BombConsumable>(pos, 1, this->tileatlas_Texture, source_rect); // Fügt standardmäßig 1 Bombe hinzu
                        }
                    }
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