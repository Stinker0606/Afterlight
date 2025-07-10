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
    Vector2 mpp=pao.Get_Player_Pos();
    this->cam.target= (Vector2){mpp.x-100,mpp.y-50};
}
