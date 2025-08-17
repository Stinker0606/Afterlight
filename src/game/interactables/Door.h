#pragma once
#include "../Collidable.h"
#include <string>

/**
 * @brief Repräsentiert eine Tür (door), die den Spieler zu einer neuen Map teleportiert.
 * Dieses Objekt ist eine unsichtbare Trigger-Zone.
 */
class Door : public Collidable
{
private:
    std::string target_map_;
    std::string target_spawn_point_;

public:
    /**
     * @param hitbox Das Rechteck aus Tiled, das den Trigger-Bereich definiert.
     * @param target_map Der Dateiname der Ziel-Map (z.B. "Level2.json").
     * @param target_spawn_point Der Name des Spawn-Punkt-Objekts in der Ziel-Map.
     */
    Door(Rectangle hitbox, std::string target_map, std::string target_spawn_point);
    ~Door() override = default;

    // Türen haben keine eigene Logik pro Frame und werden nicht gezeichnet.
    void Tick(float delta_time) override {};
    void Draw() override {};
    void On_Collision(std::shared_ptr<Collidable> other) override {};

    // Eine neue Art von Kollisionstyp, damit der Spieler sie erkennt.
    Collision_Type Get_Collision_Type() const override;

    // Getter, damit der Spieler die Ziel-Infos auslesen kann
    const std::string& Get_Target_Map() const { return target_map_; };
    const std::string& Get_Target_Spawn_Point() const { return target_spawn_point_; };
};