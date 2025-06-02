#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <../../external/tileson/tileson.hpp>


class Screen {
public:
    Screen();
    ~Screen();
    void draw_Level() const;

private:
    std::unique_ptr<tson::Map> map;
    Texture2D tileatlas_Texture;
    std::string levelmap_Path ="assets/maps/test_levelmap.json";
};


#endif //SCREEN_H