#include <iostream>
#include <fstream>
#include <string>
#include "Screen.h"
#include "../Vectors.h"
#include <vector>
#include "../config.h.in"
#include "GameScene.h"

// in main erstellen: object_Manager g_objectManager;

Screen::Screen(int *level_Ptr) : Level_Nbr_Ptr(level_Ptr) {
    this->loaded= false;
}

Screen::~Screen() {
    if (tileatlas_Texture.id != 0) {
        UnloadTexture(tileatlas_Texture);
    }
}

void Screen::Load_Levelmap() {
    int Level = *Level_Nbr_Ptr;
    tson::Tileson parser;
    std::string levelmap_Path = game::Config::GetLevelMapPath(Level);

    map = parser.parse(levelmap_Path);

    if (map == nullptr || map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Failed to load or parse map: " << levelmap_Path << std::endl;
        std::cerr << "Status: " << map->getStatusMessage() << "\n";
        return;
    }

    // Load texture
    for (auto &tileset: map->getTilesets()) {
        std::string image_Path_Raw = tileset.getImagePath().string();

        if (image_Path_Raw.substr(0, 3) == "../") {
            image_Path_Raw = image_Path_Raw.substr(3);
        }

        std::string image_Path = "../../assets/Tiled/" + image_Path_Raw;
        tileatlas_Texture = LoadTexture(image_Path.c_str());

        if (tileatlas_Texture.id == 0) {
            std::cerr << "Failed to load tile atlas texture: " << image_Path << std::endl;
        }
        break;
    }
}

//Every Tilelayer need to posses a boolean IsAboveObjects = true if it should be rendered after the objects
//Draw_Level() function calls now need to look loke this
//
//Screen.Draw_Level(false)
//Screen.LoadGameObjects()
//Screen.Draw_Level(true)

//Draw_Level(false) only draws Tilelayer that are below the objects
//while Draw_Level(true) only draws the ones above the Objects

void Screen::Draw_Level(std::shared_ptr<Cam> kamera,bool aboveObjects) {


    //make sure the map is loaded before drawing
    if (map == nullptr) {
        std::cerr << "Cannot draw level: Map not loaded." << std::endl;
        return;
    }

    //Iterate through the map layers
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

        //get Tilelayer Data
        auto &tile_Layer = layer.getTileData();

        //Iterate through each Tile
        for (const auto &pair: tile_Layer)
            //'pair.first' is a std::tuple<int, int> representing the Tile's grid position
            //'pair.second' is a pointer to the tson::Tile object
        {
            // Extract x and y coordinates from the tuple
            int x = std::get<0>(pair.first);
            int y = std::get<1>(pair.first);
            tson::Tile *tile = pair.second;

            if (tile != nullptr) {
                //Check if there is a tile at the current posiotion
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

void Screen::LoadGameObjects(Object_Manager& g_objectManager) {
    if (!this->loaded){
        Load_Levelmap();
    }
    if (map == nullptr) {
        std::cerr << "Kann Spielobjekte nicht laden: Karte ist nicht geladen" << std::endl;
    }

    //Processing Object layers
    for (auto &layer: map->getLayers()) {
        if (layer.getType() == tson::LayerType::ObjectGroup) {
            const std::string &layer_Name = layer.getName();

            for (auto &object: layer.getObjects()) {
                if (layer_Name == "walls") {
                    Vector2 temp_pos;
                    temp_pos.x = static_cast<float>(object.getPosition().x);
                    temp_pos.y = static_cast<float>(object.getPosition().y);
                    Vector2 temp_size;
                    temp_size.x = static_cast<float>(object.getSize().x);
                    temp_size.y = static_cast<float>(object.getSize().y);

                    Walls *new_wall = new Walls(temp_pos, temp_size /*, ggf. weitere Konstruktor-Parameter */);
                    g_objectManager.AddObject(new_wall);
                    std::cout << "Wand zu Object Manager hinzugefügt: " << new_wall << std::endl;
                } else if (layer_Name == "consumable") {
                    Vector2 temp_pos;
                    temp_pos.x = static_cast<float>(object.getPosition().x);
                    temp_pos.y = static_cast<float>(object.getPosition().y);

                    Consumables *new_consumable = new Consumables(
                            temp_pos, object.getName() /*, ggf. weitere Konstruktor-Parameter */);
                    g_objectManager.AddObject(new_consumable);
                    std::cout << "Verbrauchsgegenstand zu Object Manager hinzugefügt: " << new_consumable << std::endl;
                }


                if (object.getGid() > 0) {
                    tson::Tile *tile = nullptr;
                    for (auto &tileset: map->getTilesets()) {
                        if (object.getGid() >= tileset.getFirstgid() &&
                            object.getGid() < (tileset.getFirstgid() + tileset.getTileCount())) {
                            uint32_t localTileId = object.getGid() - tileset.getFirstgid();
                            tile = tileset.getTile(localTileId);
                            if (tile != nullptr) break;
                        }
                    }
                    if (tile != nullptr) {
                        tson::Rect drawingRect = tile->getDrawingRect();
                        Rectangle srcRect = {
                                static_cast<float>(drawingRect.x), static_cast<float>(drawingRect.y),
                                static_cast<float>(drawingRect.width), static_cast<float>(drawingRect.height)
                        };
                        Vector2 destPos = {
                                static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y)
                        };
                        DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
                    }
                }
            }
        }
    }
}