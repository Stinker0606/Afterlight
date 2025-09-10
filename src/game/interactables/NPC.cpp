#include "NPC.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"
#include "raylib.h"

// Alle Werte sind jetzt hier fest im Code definiert
const int NPC_ANIM_FRAMES = 20;
const Vector2 NPC_ANIM_SIZE = { 100, 100 };
const char* NPC_SPRITE_PATH = "assets/graphics/animations/John/John_Idle_Front.png";

// Definiere die Geschwindigkeit für jeden Frame.
const std::vector<int> NPC_IDLE_TIMINGS = {
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};


NPC::NPC(Vector2 position)
    : animation_(NPC_ANIM_SIZE, NPC_SPRITE_PATH, NPC_ANIM_FRAMES, NPC_ANIM_FRAMES, NPC_IDLE_TIMINGS, true)
{
    dia_trigger = AssetManager::GetInstance().Load("assets/graphics/ui/E_DiaTrig.png");

    hitbox = { position.x, position.y, 32, 48};
    useFog = true;
}

void NPC::Tick(float delta_time) {
    is_in_range_ = false;
}

void NPC::Draw() {
    Vector2 base_position = {
        hitbox.x + (hitbox.width / 2.0f),
        hitbox.y + (hitbox.height / 2.0f)
    };

    Vector2 draw_position = {
        base_position.x + 2.0f,
        base_position.y - 0.0f
    };

    animation_.Draw_Current_Frame(draw_position, Fade(WHITE, visibility_alpha));
    animation_.Next_Frame();

    if (is_in_range_ && !has_given_key_) {
        // Die "E"-Anzeige wird jetzt auch sauber zentriert über dem NPC gezeichnet.
        DrawTextureV(dia_trigger, {hitbox.x + hitbox.width / 2.0f - dia_trigger.width / 2.0f, hitbox.y - 20}, WHITE);
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