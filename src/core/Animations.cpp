//
// Created by Kruse on 02/07/2025.
//


#include "Animations.h"
Animations::Animations(Vector2 sprite_size,const char* filename,int FC,int spl) {
    this->spritesheet= LoadTexture(filename);
    this->size=sprite_size;
    this->frame_Count=FC;
    this->sprites_per_line=spl;
}
void Animations::First_Frame() {
    target.x=0;
    target.y=0;
    this->current_Frame=0;
}
void Animations::Next_Frame() {
    this->current_Frame++;
    if (this->frame_Count> this->current_Frame) {
        if (this->current_Frame % this->sprites_per_line == 0 && current_Frame != 0) {
            this->target.x = 0;
            this->target.y = this->target.y + this->size.y;
        } else {
            this->target.x = this->target.x + this->size.x;
        }
    }
}
void Animations::Draw_Current_Frame(Vector2 pos) {
    DrawTextureRec(spritesheet,target,pos,WHITE);
}