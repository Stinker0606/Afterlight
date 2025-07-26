#include "LevelScreen.h"
#include <iostream>
#include <string>
#include <vector>
#include "../config.h.in"
#include "../game/Walls.h"
#include "../game/Spawner/Level1Spawner.h"

// Konstruktor ist identisch zur originalen Screen-Klasse
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

// Draw_Level ist identisch
void LevelScreen::Draw_Level(std::shared_ptr<Cam> kamera, bool aboveObjects) {
    if (map == nullptr) {
        return;
    }

    BeginMode2D(kamera->cam);
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
        for (const auto &pair: tile_Layer)
        {
            int x = std::get<0>(pair.first);
            int y = std::get<1>(pair.first);
            tson::Tile *tile = pair.second;

            if (tile != nullptr) {
                tson::Rect drawingRect = tile->getDrawingRect();
                tson::Vector2f worldPos = {
                        static_cast<float>(x * map->getTileSize().x),
                        static_cast<float>(y * map->getTileSize().y)
                };
                Rectangle srcRect = {
                        static_cast<float>(drawingRect.x),
                        static_cast<float>(drawingRect.y),
                        static_cast<float>(drawingRect.width),
                        static_cast<float>(drawingRect.height)
                };
                Vector2 destPos = {
                        static_cast<float>(worldPos.x),
                        static_cast<float>(worldPos.y)
                };
                DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
            }
        }
    }
    EndMode2D();
}

// LoadGameObjects ist vorerst eine saubere Basis.
// HIER werden wir in Phase 1.3 unsere Erweiterungen für Spawner, Items etc. einfügen.
void LevelScreen::LoadGameObjects(Object_Manager& g_objectManager) {
    if (!this->loaded){
        Load_Levelmap();
        this->loaded = true; // Sicherstellen, dass die Map nur einmal geladen wird
    }
    if (map == nullptr) {
        std::cerr << "Kann Spielobjekte nicht laden: Karte ist nicht geladen" << std::endl;
        return;
    }

    // Temporäre Listen für das alte Spawner-System.
    // Wir müssen sie erstellen, auch wenn wir sie nicht direkt benutzen,
    // weil der `EnemySpawner`-Konstruktor sie erwartet.
    static std::vector<Rectangle> temp_obstacle_list;
    static std::vector<enemy::Enemy_Base_Class*> temp_raw_enemy_list;

    // Verarbeite alle Objekt-Layer
    for (auto &layer: map->getLayers())
    {
        if (layer.getType() == tson::LayerType::ObjectGroup)
        {
            // Lade Standard-Objekte, die die Engine bereits kennt
            if (layer.getName() == "walls") {
                for (auto &object: layer.getObjects()) {
                    // Wir prüfen hier den OBJEKT-NAMEN, nicht den Layer-Namen!
                    if (object.getName() == "walls") {
                        Vector2 temp_pos = { (float)object.getPosition().x, (float)object.getPosition().y };
                        Vector2 temp_size = { (float)object.getSize().x, (float)object.getSize().y };
                        g_objectManager.AddObject(std::make_shared<Walls>(temp_pos, temp_size));
                    }
                    // HIER werden wir später `else if (object.getName() == "movWall")` etc. hinzufügen
                }
            }
            // ---------------------------------------------------------------------
            else if (layer.getName() == "spawner")
            {
                for (auto &object: layer.getObjects())
                {
                    if (object.getName() == "spawn1") {

                        Rectangle spawner_area = {
                            (float)object.getPosition().x,
                            (float)object.getPosition().y,
                            (float)object.getSize().x,
                            (float)object.getSize().y
                        };

                        float spawn_rate = 0.5f;
                        int max_enemies = 5;

                        if(object.getProperties().hasProperty("spawn_rate"))
                            spawn_rate = object.getProperties().getValue<float>("spawn_rate");
                        if(object.getProperties().hasProperty("max_enemies"))
                            max_enemies = object.getProperties().getValue<int>("max_enemies");

                        // KORREKTE ERSTELLUNG:
                        // Wir erstellen den Spawner mit `new` und übergeben ihn dann an einen `std::shared_ptr`.
                        // Das stellt sicher, dass der korrekte 5-Argumente-Konstruktor aufgerufen wird.
                        std::shared_ptr<Level1_Spawner> spawner_obj(new Level1_Spawner(
                            spawner_area,
                            temp_obstacle_list,
                            temp_raw_enemy_list,
                            spawn_rate,
                            max_enemies
                        ));

                        // Füge den Spawner als `Collidable` zum ObjectManager hinzu.
                        g_objectManager.AddObject(spawner_obj);
                    }
                }
            }
        }
    }
}