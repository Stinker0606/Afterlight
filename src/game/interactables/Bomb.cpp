#include "Bomb.h"
#include "Explosion.h"
#include "../../scenes/LevelScene.h"
#include "../../config.h.in"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

Bomb::Bomb(Vector2 position, game::scenes::Level1Scene* scene, Object_Manager& om)
    : scene_context(scene), om_ref_(om)
{
    texture = AssetManager::GetInstance().Load("assets/graphics/projectiles/player/Bombe.png");
    hitbox = { position.x, position.y, 30.0f, 30.0f };
    useFog = true;
    detonation_timer = game::Config::kBombDetonationTime;
    blink_timer = game::Config::kBombBlinkInterval;
    is_visible = true;
}

Bomb::~Bomb()
{
   /* if (texture.id > 0) {
        UnloadTexture(texture);
    } */
}

void Bomb::Tick(float delta_time)
{
    detonation_timer -= delta_time;
    blink_timer -= delta_time;

    // Logik für das Blinken
    if (blink_timer <= 0.0f) {
        is_visible = !is_visible;
        // Das Blinken wird schneller, je näher die Detonation rückt
        blink_timer = game::Config::kBombBlinkInterval * (detonation_timer / game::Config::kBombDetonationTime);
    }

    // Detonation
    if (detonation_timer <= 0.0f) {
        Detonate();
    }
}

void Bomb::Draw()
{
    if (is_visible) {
        DrawTexture(texture, hitbox.x, hitbox.y, Fade(WHITE, visibility_alpha));
    }
}

void Bomb::Detonate()
{
    SoundManager::GetInstance().PlaySfx("bomb_explosion");

    if (scene_context) {
        // Erzeuge eine Explosion an der Position der Bombe
        auto explosion = std::make_shared<Explosion>(Vector2{hitbox.x, hitbox.y}, om_ref_);
        scene_context->Add_Object_To_Waitlist(explosion);
    }
    // Markiere die Bombe selbst zur Zerstörung
    this->Mark_For_Destruction();
}

void Bomb::On_Collision(std::shared_ptr<Collidable> other) {}

Collision_Type Bomb::Get_Collision_Type() const {
    return Collision_Type::NONE; // Hat keine Kollision, um Glitches zu vermeiden
}