//
// Created by Kruse on 06/05/2025.
//

#include "Cam.h"
#include <raylib.h>

#include "Cam.h"
#include "config.h"

#include <raylib.h>
#include <cmath>
#include <iostream>

Cam::Cam(Player_Base_Class& mp):pao(mp) {

}
void Cam::Cam_Movement(double dtm) {
    Vector2 mpp=pao.Get_Player_Center();
    int temp= game::Config::kStageWidth/2;
    this->cam.target= mpp;
    this->cam.offset.x=game::Config::kStageWidth/2;
    this->cam.offset.y=game::Config::kStageHeight/2;
}//(Vector2){mpp.x+game::Config::kStageWidth/2,mpp.y+game::Config::kStageHeight/2}
