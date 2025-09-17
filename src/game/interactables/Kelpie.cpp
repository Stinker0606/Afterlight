#include "Kelpie.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"
#include "raylib.h"

const int KELPIE_ANIM_FRAMES = 20; // animation frames, how many total
const Vector2 KELPIE_ANIM_SIZE = { 100, 100 }; // Animation size
const char* KELPIE_SPRITE_PATH = "assets/graphics/animations/Kelpie/Kelpie.png"; // Path to Kelpie animation

const std::vector<int> KELPIE_IDLE_TIMINGS = { }; // Kelpie timing


KELPIE::KELPIE(Vector2 position)
    : animation_(KELPIE_ANIM_SIZE, KELPIE_SPRITE_PATH, KELPIE_ANIM_FRAMES, KELPIE_ANIM_FRAMES, KELPIE_IDLE_TIMINGS, true)
{
    hitbox = { position.x, position.y, 32, 48};
    useFog = true;
}

void KELPIE::Tick(float delta_time) {}

void KELPIE::Draw() {
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
    }


void KELPIE::On_Collision(std::shared_ptr<Collidable> other) {}

Collision_Type KELPIE::Get_Collision_Type() const {
    return Collision_Type::WALL; // NPCs sind solide Objekte
}