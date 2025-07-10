#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <../../external/tileson/tileson.hpp>
#include "Cam.h"
#include "Object_Manager.h"
#include "CollisionManager.h"


class Screen
{
public:
    Screen(int* Level_Ptr);
    ~Screen();
    void Draw_Level(std::shared_ptr<Cam>, bool aboveObjects) ;
    void LoadGameObjects(Object_Manager& g_objectManager);
    void Load_Levelmap();

private:
    bool includeEnemySpawnersAsCollidables=false;
    Texture2D tileatlas_Texture;
    std::unique_ptr<tson::Map> map;
    int* Level_Nbr_Ptr = nullptr;
    bool loaded;

};


#endif //SCREEN_H