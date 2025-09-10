#include "NPC.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"
#include "raylib.h"

// Alle Werte sind jetzt hier fest im Code definiert
const char* NPC_SPRITE_PATH = "assets/graphics/npcs/John_Idle_Front.png";
const int NPC_ANIM_FRAMES = 19;
const Vector2 NPC_ANIM_SIZE = { 100, 100 };

// Definiere die Geschwindigkeit für jeden Frame.
const std::vector<int> NPC_IDLE_TIMINGS = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

NPC::NPC(Vector2 position)
    : animation_(NPC_ANIM_SIZE, NPC_SPRITE_PATH, NPC_ANIM_FRAMES, NPC_ANIM_FRAMES, NPC_IDLE_TIMINGS, true)
{
    hitbox = { position.x, position.y, NPC_ANIM_SIZE.x, NPC_ANIM_SIZE.y };
    useFog = true;
}

void NPC::Tick(float delta_time) {
    is_in_range_ = false;
}

void NPC::Draw() {
    animation_.Draw_Current_Frame({ hitbox.x, hitbox.y }, Fade(WHITE, visibility_alpha));
    animation_.Next_Frame();

    if (is_in_range_ && !has_given_key_) {
        DrawTextureV(dia_trigger,{hitbox.x + hitbox.width / 2, hitbox.y - 20}, WHITE);
    }
}

void NPC::On_Collision(std::shared_ptr<Collidable> other) {
    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        is_in_range_ = true;
        if (IsKeyPressed(game::Config::key_Interact) && !has_given_key_) {
            player->Add_Key(1);
            has_given_key_ = true;
            SoundManager::GetInstance().PlaySfx("item_pickup_key");
        }
    }
}

Collision_Type NPC::Get_Collision_Type() const {
    return Collision_Type::WALL; // NPCs sind solide Objekte
}