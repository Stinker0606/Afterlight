#pragma once
#include "../Collidable.h"

class BreakableWall : public Collidable
{
private:
    Texture2D tileset_texture;
    Rectangle texture_source_rect;

public:
    /**
     * @brief Konstruktor für einen schiebbaren Block.
     * @param position Die Startposition des Blocks in der Welt.
     * @param tileset Die Textur des gesamten Tilesets.
     * @param source_rect Der genaue Ausschnitt im Tileset für die Grafik dieses Blocks.
     */
    BreakableWall(Vector2 position, Texture2D tileset, Rectangle source_rect);
    ~BreakableWall() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    /**
     * @brief Zerstört die Wand und spielt den Sound ab.
     * Wird von der Explosion aufgerufen.
     */
    void DestroyWall();
};