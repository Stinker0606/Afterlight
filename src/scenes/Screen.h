#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <../../external/tileson/tileson.hpp>


class Screen
{
public:
    Screen();
    ~Screen();
    void Draw_Level() const;

private:
    std::unique_ptr<tson::Map> map;
    Texture2D tileatlas_Texture;
    std::string levelmap_Path ="assets/graphics/data/Testmap1.json";
};


#endif //SCREEN_H