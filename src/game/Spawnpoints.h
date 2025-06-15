//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_SPAWNPOINTS_H
#define RAYLIBSTARTER_SPAWNPOINTS_H


#include "../game/Objects.h"

class Spawnpoints : public Objects
{
private:
    int spawner_Hits; // Die Anzahl der Treffer, die der Spawner aushält

public:
    Spawnpoints(Vector2 position, int hits); // Angepasster Konstruktor

    // Implementierung der pure virtual functions
    CollisionType Get_Collision_Type() const override;
    bool Is_Destroyed() const;

    void Tick(float delta_time);
    void On_Collision(Collidable* other) override;
    void Draw() override;
};



#endif //RAYLIBSTARTER_SPAWNPOINTS_H
