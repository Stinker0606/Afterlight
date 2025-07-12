//
// Created by Kruse on 06/05/2025.
//

#ifndef RAYLIBSTARTER_CAM_H
#define RAYLIBSTARTER_CAM_H
#include <raylib.h>
#include "../game/PlayerBaseClass.h"

struct Cam
{public:
    Camera2D cam={0,0,game::Config::cam_start_pos_x,game::Config::cam_start_pos_y,0,game::Config::cam_zoom};
    Player_Base_Class& pao;

    Cam(Player_Base_Class&);
    void Cam_Movement(double dtm);
};




#endif //RAYLIBSTARTER_CAM_H