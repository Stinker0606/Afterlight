//
// Created by Kruse on 02/06/2025.
//

#include "Objects.h"
Objects::Objects(Vector2 position, std::string name)
{
    this->name=name;
    this->pos.x=position.x ;
    this->pos.y=position.y;
}
void Objects::draw() {
    DrawTexture(this->text, this->pos.x, this->pos.y,WHITE);
}