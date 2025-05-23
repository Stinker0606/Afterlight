//
// Created by Kruse on 23/05/2025.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "screen.h"

Screen::Screen() {

    //load tileset json
    std::ifstream tileset_Description_File("assets/maps/test_Tileset.json");
    if (tileset_Description_File.is_open()) {
        tileset_Description_File >> tileset_Description;
        tileset_Description_File.close();
    } else {
        std::cerr << "Failed to open tileset description file!" << std::endl;
    }

    //load level map
    std::ifstream levelmap_File("assets/maps/test_Levelmap.json");
    if (levelmap_File.is_open()) {
        levelmap_File >> levelmap;
        levelmap_File.close();
    } else {
        std::cerr << "Failed to open level map description File" << std::endl;
    }

    // Load texture using from "image" keyword from tileset_Description
    std::string image_Path = tileset_Description["image"];
    tileatlas_Texture = LoadTexture(image_Path.c_str());

    columns = tileset_Description["columns"];

}

Screen::~Screen() {
    UnloadTexture(tileatlas_Texture);
}

void Screen::draw_Level() const {


    auto layer_Count = levelmap["layers"].size();
    for (int i = 0; i < layer_Count; ++i)
    {
        auto layer = levelmap["layers"][i];
        int mapWidth = layer["width"];
        int mapHeight = layer["height"];
        // Draw the loaded map
        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                int index = y * mapWidth + x;
                int tiledID = layer["data"][index];
                if (tiledID == 0) continue; // Skip empty tiles

                int tiledIndex = tiledID - 1;
                Rectangle srcRect = {
                        static_cast<float>((tiledIndex % columns) * 16), // Assuming tile size is 32x32
                        static_cast<float>((tiledIndex / columns) * 16),
                        (float)16, (float)16
                };

                Vector2 pos = { (float)(x * 16) * 1.0f, (float)(y * 16) * 1.0f };
                // Draw the tile
                DrawTextureRec(tileatlas_Texture, srcRect, pos, WHITE);
            }
        }
    }
}
