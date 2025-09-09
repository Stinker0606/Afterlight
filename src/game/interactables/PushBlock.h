#pragma once

#include "../Collidable.h"
#include "raylib.h"

/**
 * @brief Repräsentiert einen schiebbaren Block (movWall).
 * Dieses Objekt ist ein "Tile-Objekt", d.h., seine Grafik wird direkt
 * aus dem Tileset der Map entnommen.
 */
class Push_Block : public Collidable
{
private:
    // Die Textur des gesamten Tilesets der Map.
    Texture2D tileset_texture;
    // Das spezifische Rechteck im Tileset, das diesen Block darstellt.
    Rectangle texture_source_rect;
    // Speichert die vorherige Position, um bei Kollisionen zurückgesetzt zu werden.
    Vector2 previous_position;

public:
    /**
     * @brief Konstruktor für einen schiebbaren Block.
     * @param position Die Startposition des Blocks in der Welt.
     * @param tileset Die Textur des gesamten Tilesets.
     * @param source_rect Der genaue Ausschnitt im Tileset für die Grafik dieses Blocks.
     */
    Push_Block(Vector2 position, Texture2D tileset, Rectangle source_rect);
    ~Push_Block() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    /**
     * @brief Versucht, den Block in eine Richtung zu bewegen.
     * @param move_direction Der normalisierte Vektor der Bewegungsrichtung.
     */
    void Push(Vector2 move_direction);
};