#include "Trigger2.h"
#include "../PlayerClass.h"

Trigger2::Trigger2(Rectangle hitbox)
{
    this->hitbox = hitbox;
    dia_trigger2 = AssetManager::GetInstance().Load("assets/graphics/ui/Q_UITrig.png");

}

void Trigger2::Tick(float delta_time) {
    is_in_range_ = false;
}

void Trigger2::Draw() {
    if (is_in_range_) {
        // DrawText("E", hitbox.x + hitbox.width / 2, hitbox.y - 20, 20, Color { 216, 176, 168, 255 });
        DrawTextureV(dia_trigger2,{hitbox.x + hitbox.width / 2, hitbox.y - 20}, WHITE);
    }
}

void Trigger2::On_Collision(std::shared_ptr<Collidable> other) {
    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        is_in_range_ = true;
    }
}

Collision_Type Trigger2::Get_Collision_Type() const {
    return Collision_Type::NONE;
}
