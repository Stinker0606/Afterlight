#pragma once

#include "../Collidable.h"
#include <raylib.h>

class Player_Base_Class;

class HealConsumable : public Collidable
{
private:
    Texture2D tileset_texture;
    Rectangle texture_source_rect;
    int heal_amount;

public:
    /**
     * @brief Konstruktor für ein Heil-Item, das als Tile-Objekt geladen wird.
     * @param position Die Startposition in der Welt.
     * @param healAmount Die Menge an HP die wiederhergestellt wird.
     * @param tileset Die Textur des gesamten Tilesets der Map.
     * @param source_rect Der genaue Ausschnitt im Tileset für die Grafik dieses Items.
     */
    HealConsumable(Vector2 position, int healAmount, Texture2D tileset, Rectangle source_rect);
    ~HealConsumable() override;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    void ApplyEffect(Player_Base_Class* player);
};