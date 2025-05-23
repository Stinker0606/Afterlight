//
// Created by Kruse on 10/04/2025.
//
#include <chrono>
#include "DeltaTimeMachine.h"
namespace DT{
    void timemachine::Start() {                                  //gets the current time and saves it as frame1
        this->frame1= std::chrono::high_resolution_clock::now();
    }

    void timemachine::Update() {                                //gets the current time again saves it as frame2 then calculates the difference to get the actual frame duration
                                                                //it converts this time into milliseconds and from there into a float where 1s = 1.0
                                                                // it then saves frame2 into frame one as the new reference point
                                                                // BE AWARE: delta time is always one frame behind the actual game so any lag spikes will be dealt with one frame later
                                                                // if you want an explanation https://www.youtube.com/watch?v=yGhfUcPjXuE
        this->frame2= std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds deltaTime= std::chrono::duration_cast<std::chrono::milliseconds>(frame2-frame1);
        this->dt= deltaTime.count();
        if (dt==0){
            dt=1;
        }
        this->dt=dt/100;
        this->frame1=this->frame2;
    }

    float timemachine::Get_Dt() {
        return this->dt;
    }
};