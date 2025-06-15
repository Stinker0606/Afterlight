//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_CONSUMABLES_H
#define RAYLIBSTARTER_CONSUMABLES_H


#include "Collidable.h"

class Collision_Manager;
class Consumables : public Collidable
{
protected:
    Rectangle hitbox;
    Texture2D texture;
    std::string name;
    Collision_Manager* manager_ptr; // NEU: Pointer auf den Manager

public:
    // NEU: Konstruktor akzeptiert jetzt einen Manager-Pointer
    Consumables(Vector2 position, std::string name, Collision_Manager* manager);
    // NEU: Destruktor zum Abmelden
    ~Consumables();

    // Implementierung der Collidable-Schnittstelle
    Rectangle Get_Hitbox() const override;
    CollisionType Get_Collision_Type() const override;

    void Tick(float delta_time);
    void On_Collision(Collidable* other) override;
    void Draw();
};


#endif //RAYLIBSTA