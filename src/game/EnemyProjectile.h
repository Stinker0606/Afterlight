//
// Created by Manza on 6/22/2025.
//

#pragma once
#include "raylib.h"
#include "Collidable.h"

namespace game {
    /**
     * @brief Stellt ein einzelnes Projektil dar, das von einem Gegner abgefeuert wird.
     * Erbt von Collidable, um mit der Spielwelt (Wände, Spieler) interagieren zu können.
     */
    class Enemy_Projectile : public Collidable {
    public:
        Vector2 position;
        Vector2 velocity;
        bool is_active;
        int damage;
        Texture2D sprite;
        Rectangle hitbox;

        /**
         * @brief Konstruktor für ein neues Gegner-Projektil.
         * @param start_position Die Startposition des Projektils.
         * @param direction Der normalisierte Richtungsvektor.
         * @param projectile_speed Die Geschwindigkeit des Projektils.
         * @param damage Der Schaden, den das Projektil verursacht.
         * @param sprite_path Der Pfad zur Sprite-Textur des Projektils.
         */
        Enemy_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path);
        ~Enemy_Projectile();

        void Tick(float delta_time);
        void Draw() const;


        Collision_Type Get_Collision_Type() const override;
        void On_Collision(Collidable* other) override;
    };
}