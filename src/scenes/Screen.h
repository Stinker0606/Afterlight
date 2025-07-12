#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <../../external/tileson/tileson.hpp>
#include "Cam.h"
#include "Object_Manager.h"
#include "FogManager.h"

class Screen
{
public:
    FogManager fogManager; // Muss public sein

    Screen(int* Level_Ptr);
    ~Screen();
    void Draw_Level(std::shared_ptr<Cam>, bool aboveObjects) ;
    void LoadGameObjects(Object_Manager& g_objectManager);
    void UpdateFog(Vector2 playerPosition, float deltaTime);

private:
    Texture2D tileatlas_Texture;
    std::unique_ptr<tson::Map> map;
    int* Level_Nbr_Ptr = nullptr;
    bool loaded;
    void Load_Levelmap();
};

#endif //SCREEN_H