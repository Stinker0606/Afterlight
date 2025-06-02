//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
Walls::Walls(Vector2 position, Vector2 size) {

    this->size=size;
    this->pos=position;
    Rectangle hb(this->pos.x, this->pos.y, this->size.x, this->size.y);
    this->hitbox=hb;

}

void Walls::draw() {
}