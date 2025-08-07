#pragma once

#include "raylib.h"
#include "Collidable.h"
#include "Object_Manager.h"
#include "../config.h.in"
#include "FacingDirection.h"

namespace game {
    /**
     * @brief Stellt ein einzelnes Projektil dar, das vom Spieler abgefeuert wird.
     * Erbt von Collidable, um mit der Spielwelt (Wände, Gegner) interagieren zu können.
     */
    class Player_Projectile : public Collidable {
    public:
        Vector2 position;
        Vector2 velocity;
        bool is_active;
        int damage;
        Texture2D sprite;
        Rectangle hitbox;

        // Eine Variable, um die Lebensdauer des Projektils zu speichern.
        float lifetime;

        // Wir speichern die Blickrichtung, um die richtige Grafik und Rotation zu wählen
        Facing_Direction facing_direction;
        float rotation;

        /**
         * @brief Konstruktor für ein neues Spieler-Projektil.
         * @param start_position Die Startposition des Projektils.
         * @param direction Der normalisierte Richtungsvektor.
         * @param projectile_speed Die Geschwindigkeit des Projektils.
         * @param damage Der Schaden, den das Projektil verursacht.
         * @param sprite_path Der Pfad zur Sprite-Textur des Projektils.
         */
        Player_Projectile(Vector2 start_position, Vector2 direction, int damage, Facing_Direction facing_dir);
        ~Player_Projectile();

        void Tick(float delta_time) override;
        void Draw() override;

        Collision_Type Get_Collision_Type() const override;
        void On_Collision(std::shared_ptr<Collidable> other) override;
    };
}