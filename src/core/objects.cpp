//
// Created by Kruse on 02/06/2025.
//

#include "objects.h"
objects::objects(Vector2 position, std::string name) {
    this->name=name;
    this->pos.x=position.x ;
    this->pos.y=position.y;
}
void objects::draw() {
    DrawTexture(this->text, this->pos.x, this->pos.y,WHITE);
}