#pragma once

#include "../Collidable.h"
#include <raylib.h>

class Player_Base_Class;

class KeyConsumable : public Collidable
{
private:
    Texture2D tileset_texture;
    Rectangle texture_source_rect;
    int key_amount;

public:
    /**
     * @brief Konstruktor für ein Key-Item, das als Tile-Objekt geladen wird.
     * @param position Die Startposition in der Welt.
     * @param amount Die Menge an Schlüssel die im Inventar hinzugefügt werden.
     * @param tileset Die Textur des gesamten Tilesets der Map.
     * @param source_rect Der genaue Ausschnitt im Tileset für die Grafik dieses Items.
     */
    KeyConsumable(Vector2 position, int amount, Texture2D tileset, Rectangle source_rect);
    ~KeyConsumable() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};