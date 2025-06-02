#include <iostream>
#include <fstream>
#include <string>
#include "screen.h"
#include "../Vectors.h"
#include "Walls.h"
#include <vector>

Screen::Screen() {

    //Create a Tileson parser Object
    tson::Tileson parser;

    //Attempt to parse Tiled map file
    //the parse function return a std::unique_ptr<tson::Map>
    map = parser.parse(levelmap_Path);

    //Check if the map was loaded successfully
    if (map == nullptr)
    {
        std::cerr << "Failed to open or parse level map" << levelmap_Path << std::endl;
        return;
    }

    //Check if the map version is supported by Tileson
    if (map->getStatus() != tson::ParseStatus::OK)
    {
        std::cerr << "Error parsing map: " << map->getStatusMessage() << std::endl;
        return;
    }

    //Iterate through all tilesets defined in the Tile map
    //A Tilemap can have multiple tilesets. We're loading the texture of the first one
    for (auto& tileset : map->getTilesets())
    {
        std::string image_Path = "assets/maps/" + tileset.getImagePath().string();

        //load texture and check if loading was successfull
        tileatlas_Texture = LoadTexture(image_Path.c_str());
        if (tileatlas_Texture.id == 0)
        {
            std::cerr << "Failed to load tile atlas texture: " << image_Path << std::endl;
        }
        //breaking after one tileset_Texture
        break;
    }
}

Screen::~Screen() {
    if (tileatlas_Texture.id != 0)
    {
        UnloadTexture(tileatlas_Texture);
    }
}

void Screen::draw_Level() const {

    //make sure the map is loaded before drawing
    if (map == nullptr)
    {
        std::cerr << "Cannot draw level: Map not loaded." << std::endl;
        return;
    }

    //Iterate through the map layers
    for (auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::TileLayer)
        {
            //get Tilelayer Data
            auto& tile_Layer = layer.getTileData();

            //Iterate through each Tile
            for (const auto& pair : tile_Layer)
                //'pair.first' is a std::tuple<int, int> representing the Tile's grid position
                //'pair.second' is a pointer to the tson::Tile object
            {
                // Extract x and y coordinates from the tuple
                int x = std::get<0>(pair.first);
                int y = std::get<1>(pair.first);
                tson::Tile* tile = pair.second;

                if (tile != nullptr) { //Check if there is a tile at the current posiotion
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
            //Processing Object layers
        else if (layer.getType() == tson::LayerType::ObjectGroup)
        {
            std::cout << "Processing Object Layer: " << layer.getName() << std::endl;

            //get the name of the current Layer
            const std::string& layer_Name = layer.getName();

            //Iterate through each object in the object group
            for (auto& object : layer.getObjects()) {
                std::cout << "Object Name: " << object.getName()
                          << ", Type: " << object.getType()
                          << ", Position: (" << object.getPosition().x << ", " << object.getPosition().y << ")"
                          << ", Size: (" << object.getSize().x << ", " << object.getSize().y << ")" << std::endl;

                //Check for certain layer Names
                //Add any specific Object layer functionalities here, depending on the type of layer
                //For example: Walls, Player Spawns, Enemy Spawns, audio/environmental triggers...
                if (layer_Name == "walls")
                {
                    Vector2 temp;
                    temp.x=object.getPosition().x;
                    temp.y=object.getPosition().y;
                    Vector2 temp2;
                    temp2.x=object.getSize().x;
                    temp2.y=object.getSize().y;
                    vec_walls.push_back(Walls(temp,temp2));
                }

                else if (layer_Name == "consumable")
                {
                    Vector2 temp;
                    temp.x=object.getPosition().x;
                    temp.y=object.getPosition().y;
                    vec_cons.push_back(consumables(temp,object.getName()));
                }

                else if (layer_Name == "levelSpawns")
                {
                    std::cout << " This layer contains level spawns." << std::endl;
                }

                else if (layer_Name == "levelSpawns")
                {
                    std::cout << " This layer contains level spawns." << std::endl;
                }


                //If the object has a gid (is a tile object), it can be drawn
                //object that are visually represented by a tile may use this
                if (object.getGid() > 0)
                {
                    tson::Tile* tile = nullptr;
                    //Iterate through the tilesets to find the one containing this gid
                    for (auto& tileset : map->getTilesets())
                    {
                        //check if gid is within this tileset
                        if (object.getGid() >= tileset.getFirstgid() &&
                            object.getGid() < (tileset.getFirstgid() + tileset.getTileCount()))
                        {
                            //calculate the local ID within this tileset
                            uint32_t localTileId = object.getGid() - tileset.getFirstgid();
                            //get the tile from the tileset using it's local Id
                            tile = tileset.getTile(localTileId);
                            if (tile != nullptr)
                            {
                                //found the tile, no need to keep searching
                                break;
                            }
                        }
                    }
                    if (tile != nullptr)
                    {
                        tson::Rect drawingRect = tile->getDrawingRect();
                        Rectangle srcRect = {
                                static_cast<float>(drawingRect.x),
                                static_cast<float>(drawingRect.y),
                                static_cast<float>(drawingRect.width),
                                static_cast<float>(drawingRect.height)
                        };
                        Vector2 destPos = {
                                static_cast<float>(object.getPosition().x),
                                static_cast<float>(object.getPosition().y)
                        };
                        DrawTextureRec(tileatlas_Texture, srcRect, destPos, WHITE);
                    }
                }
            }
        }
    }
}