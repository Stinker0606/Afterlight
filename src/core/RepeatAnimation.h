//
// Created by Kruse on 07/07/2025.
//

#ifndef RAYLIBSTARTER_REPEATANIMATION_H
#define RAYLIBSTARTER_REPEATANIMATION_H

#include <raylib.h>
#include <memory>
class RepeatAnimation {

    private:
        int current_Frame=0;
        int frame_Count;
        int sprites_per_line;
        Vector2 size;
        Rectangle target;
        Texture2D spritesheet;
    public:
    RepeatAnimation(Vector2,const char*,int,int);
        void Next_Frame();
        void First_Frame();
        void Draw_Current_Frame(Vector2);

};


#endif //RAYLIBSTARTER_REPEATANIMATION_H
