//
// Created by $Will on 11.06.2025.
//

#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "raylib.h"
#include <string>
#include <memory>

enum class Collision_Type
{
    PLAYER,
    ENEMY,
    WALL,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE,
    CONSUMABLE,
    ENEMY_SPAWNER,
    PLAYER_MELEE_HITBOX,
    ENEMY_MELEE_HITBOX,
    PORTAL
};

class Collidable : public std::enable_shared_from_this<Collidable>
{
protected:
    Rectangle hitbox;
    bool is_Marked_For_Destruction = false;

    // Bestimmt, ob dieses Objekt von der Nebel-Mechanik betroffen ist.
    // Wird in Tiled über die Custom Property "useFog" gesteuert.
    bool useFog = false;

    // Bestimmt die aktuelle Sichtbarkeit (Transparenz) des Objekts.
    // 1.0f = voll sichtbar, 0.0f = komplett unsichtbar.
    float visibility_alpha = 1.0f;

public:
    virtual ~Collidable() = default;

    Rectangle Get_Hitbox() const{return this->hitbox;};
    Vector2 Get_Hitbox_Center() const {
        return { hitbox.x + hitbox.width / 2.0f, hitbox.y + hitbox.height / 2.0f };
    }
    virtual Collision_Type Get_Collision_Type() const = 0;
    virtual void Tick(float delta_time) = 0;
    virtual void Draw()=0;
    virtual void On_Collision(std::shared_ptr<Collidable> other) = 0;
    virtual void Set_Position(Vector2 position) { hitbox.x = position.x; hitbox.y = position.y; }

    virtual void Mark_For_Destruction() { this->is_Marked_For_Destruction = true; }
    bool Is_Marked_For_Destruction() const { return this->is_Marked_For_Destruction; }


    /**
     * @brief Setzt ob dieses Objekt vom Nebel betroffen sein soll.
     * @param usefog True, wenn es betroffen sein soll, sonst false.
     */
    void Set_Use_Fog(bool use_fog_value) { this->useFog = use_fog_value; }

    /**
     * @brief Setzt die aktuelle Sichtbarkeit (Transparenz) des Objekts.
     * @param alpha Ein Wert zwischen 0.0 (unsichtbar) und 1.0 (sichtbar).
     */
    void Set_Visibility_Alpha(float alpha) { this->visibility_alpha = alpha; }

    /**
     * @brief Gibt zurück ob dieses Objekt vom Nebel betroffen ist.
     */
    bool Get_Use_Fog() const { return this->useFog; }
};
#endif //COLLIDABLE_H