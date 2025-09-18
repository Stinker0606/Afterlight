#include "DialogTrigger.h"
#include "../PlayerClass.h"
#include "../../config.h.in"

DialogTrigger::DialogTrigger(Rectangle hitbox, std::string text, std::string name, std::string portrait)
    : text_(text), name_(name), portrait_path_(portrait)
{
    this->hitbox = hitbox;
    dia_trigger = AssetManager::GetInstance().Load("assets/graphics/ui/E_DiaTrig.png");

}

void DialogTrigger::Tick(float delta_time) {
    is_in_range_ = false;
}

void DialogTrigger::Draw() {
    if (is_in_range_) {
       // DrawText("E", hitbox.x + hitbox.width / 2, hitbox.y - 20, 20, Color { 216, 176, 168, 255 });
        DrawTextureV(dia_trigger,{hitbox.x + hitbox.width / 2, hitbox.y - 20}, WHITE);
    }
}

void DialogTrigger::On_Collision(std::shared_ptr<Collidable> other) {
    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        is_in_range_ = true;
    }
}

Collision_Type DialogTrigger::Get_Collision_Type() const {
    return Collision_Type::NONE;
}

const std::string& DialogTrigger::GetText() const { return text_; }
const std::string& DialogTrigger::GetName() const { return name_; }
const std::string& DialogTrigger::GetPortraitPath() const { return portrait_path_; }