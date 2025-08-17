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

    /**
     * @brief Zeichnet den aktuellen Frame der Animation an einer Position.
     * NEU: Akzeptiert eine Tint-Farbe für visuelle Effekte wie Hit-Feedback.
     * @param pos Die Position, an der gezeichnet werden soll.
     * @param tint Die Farbe, mit der das Sprite getönt werden soll (Standard ist WHITE).
     */
    void Draw_Current_Frame(Vector2 pos, Color tint = WHITE);
};


#endif //RAYLIBSTARTER_REPEATANIMATION_H
