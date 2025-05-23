#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include <nlohmann/json.hpp>


class Screen {
public:
    nlohmann::json tileset_Description;
    nlohmann::json levelmap;
    Texture2D tileatlas_Texture{};

    Screen();
    ~Screen();

    void draw_Level() const;
    int columns;
};


#endif //SCREEN_H