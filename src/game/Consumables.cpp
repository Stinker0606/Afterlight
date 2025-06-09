//
// Created by Kruse on 02/06/2025.
//

#include "Consumables.h"
#include <iostream>
#include "../../cmake-build-debug/_deps/raylib-src/src/config.h"

Consumables::Consumables(Vector2 position, std::string name) {

    this->name=name;
    this->pos.x=position.x ;
    this->pos.y=position.y;
    this->text= LoadTexture("assets/graphics/ball.png");
    Rectangle hb(position.x,position.y, this->text.width, this->text.height);
    this->hitbox=hb;

}
void Consumables::draw() {

}