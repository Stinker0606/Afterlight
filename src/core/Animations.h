//
// Created by Kruse on 02/07/2025.
//

#ifndef RAYLIBSTARTER_ANIMATIONS_H
#define RAYLIBSTARTER_ANIMATIONS_H


#include <raylib.h>
#include <memory>

class Animations {
private:
    int current_Frame=0;
    int frame_Count;
    int sprites_per_line;
    Vector2 size;
    Rectangle target{0,0,size.x,size.y};
    Texture2D spritesheet;
public:
    Animations(Vector2,const char*,int,int);
    void Next_Frame();
    void First_Frame();
    void Draw_Current_Frame(Vector2);
};


#endif //RAYLIBSTARTER_ANIMATIONS_H
