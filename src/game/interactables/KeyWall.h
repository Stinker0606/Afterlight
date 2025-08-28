#pragma once
#include "../Collidable.h"

/**
 * @brief Repräsentiert eine verschlossene Wand/Tür (keyWall).
 * Sie ist eine unsichtbare Kollisionsbarriere, die verschwindet,
 * wenn der Spieler mit einem Schlüssel interagiert.
 */
class KeyWall : public Collidable
{
private:
    Texture2D tileset_texture;
    Rectangle texture_source_rect;

public:
    /**
     * @param hitbox Das Rechteck aus Tiled, das die Position und Größe der Barriere definiert.
     */
    KeyWall(Vector2 position, Texture2D tileset, Rectangle source_rect);
    ~KeyWall() override = default;

    void Tick(float delta_time) override {};

    void Draw() override;

    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};