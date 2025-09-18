//
// Created by Kruse on 10/04/2025.
//
#include <chrono>
#include "DeltaTimeMachine.h"
namespace DT{
    void timemachine::Start() {                                  //gets the current time and saves it as frame1
        this->frame1= std::chrono::high_resolution_clock::now();
        this->is_paused_ = false; // Sicherstellen, dass die Zeitmessung nicht pausiert startet
    }

    void timemachine::Update() {                                //gets the current time again saves it as frame2 then calculates the difference to get the actual frame duration
                                                                //it converts this time into milliseconds and from there into a float where 1s = 1.0
                                                                // it then saves frame2 into frame one as the new reference point
                                                                // BE AWARE: delta time is always one frame behind the actual game so any lag spikes will be dealt with one frame later
                                                                // if you want an explanation https://www.youtube.com/watch?v=yGhfUcPjXuE
        // --- Wenn pausiert, keine Zeit aktualisieren ---
        if (is_paused_) {
            // Setze dt auf 0, damit nichts sich bewegt, falls Get_Dt() trotzdem aufgerufen wird
            this->dt = 0.0;
            return;
        }

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
        // Gibt 0 zurück, wenn pausiert, ansonsten die korrekte Delta-Zeit
        return is_paused_ ? 0.0f : this->dt;
    }

    // --- NEUE IMPLEMENTIERUNGEN ---
    void timemachine::Pause() {
        if (!is_paused_) {
            is_paused_ = true;
            // Speichere den exakten Zeitpunkt, an dem die Pause beginnt
            pause_time_ = std::chrono::high_resolution_clock::now();
        }
    }

    void timemachine::Resume() {
        if (is_paused_) {
            // Berechne die Dauer der Pause
            auto resume_time = std::chrono::high_resolution_clock::now();
            auto pause_duration = resume_time - pause_time_;
            // Addiere die Pausendauer zum letzten Frame-Zeitpunkt.
            // Das verschiebt den Startpunkt für die nächste Berechnung nach vorne
            // und "eliminiert" so effektiv die Zeit, die in der Pause vergangen ist.
            frame1 += pause_duration;
            is_paused_ = false;
        }
    }

    bool timemachine::IsPaused() const {
        return is_paused_;
    }
};
