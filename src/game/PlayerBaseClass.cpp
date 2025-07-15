//
// Created by Kruse on 23/05/2025.
//

#include <iostream>
#include "PlayerBaseClass.h"
#include "Store.h"
#include "CollisionResponse.h"

#include <iostream>
#include <cmath>
#include "PlayerBaseClass.h"
#include "Store.h"
#include "CollisionResponse.h"

// Konstruktor
Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position, const std::vector<Collidable*>& collidables)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      player_Damage(damage),
      previous_Position(start_Position),
      collidables_in_scene(collidables),
      melee_Cooldown(0.0f), ranged_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false)
{
    hitbox={start_Position.x,start_Position.y,static_cast<float >(maintex.width),static_cast<float >(maintex.height)};
    walking_sound = LoadSound("assets/audio/sfx/foot-steps_2.mp3");
}

// Destruktor
Player_Base_Class::~Player_Base_Class()
{
}

// Phase 1 :: Player input Prüfung
void Player_Base_Class::Player_Input()
{
    if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0)
    {
        Melee_Attack();
    }
    if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0)
    {
        Ranged_Attack();
    }
}

// Phase 2 :: Verwaltung für alles was das Objekt über eine gewisse Zeit machen soll
void Player_Base_Class::Tick(float delta_time)
{
    // --- Cooldowns und Health Drain (unverändert) ---
    if (game::Config::enable_Health_Drain)
    {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }
    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (ranged_Cooldown > 0) ranged_Cooldown -= delta_time;

    // --- Schritt 1: Beabsichtigte Bewegung berechnen (dein Code, unverändert) ---
    Vector2 move_intent = {0.0f, 0.0f};
    if (IsKeyDown(game::Config::key_Up))    move_intent.y -= 1.0f;
    if (IsKeyDown(game::Config::key_Down))  move_intent.y += 1.0f;
    if (IsKeyDown(game::Config::key_Left))  move_intent.x -= 1.0f;
    if (IsKeyDown(game::Config::key_Right)) move_intent.x += 1.0f;

    is_Moving = (move_intent.x != 0.0f || move_intent.y != 0.0f);
    if (is_Moving)
    {
        move_intent = Vector2Normalize(move_intent);
        move_intent.x *= (player_Movement_Speed * delta_time);
        move_intent.y *= (player_Movement_Speed * delta_time);
    }

    // ====================================================================
    // HIER KOMMT DER SOUND-CODE HIN
    // ====================================================================
    if (is_Moving)
    {
        // Spiele den Sound nur ab, wenn er nicht bereits läuft.
        if (!IsSoundPlaying(walking_sound))
        {
            PlaySound(walking_sound);
        }
    }
    else
    {
        // Wenn der Spieler stehen bleibt, stoppe das Geräusch.
        StopSound(walking_sound);
    }
    // ====================================================================

    // --- Schritt 2: Vorausschauende Kollisionsprüfung (dein Code, unverändert) ---
    Rectangle future_hitbox = { hitbox.x + move_intent.x, hitbox.y + move_intent.y, hitbox.width, hitbox.height };

    bool would_collide_with_wall = false;
    for (const auto& other : this->collidables_in_scene)
    {
        if (other->Get_Collision_Type() == Collision_Type::WALL || other->Get_Collision_Type() == Collision_Type::ENEMY_SPAWNER) {
            if (CheckCollisionRecs(future_hitbox, other->Get_Hitbox())) {
                would_collide_with_wall = true;
                break;
            }
        }
    }

    // --- Schritt 3: Bewegen oder nicht bewegen (dein Code, unverändert) ---
    if (!would_collide_with_wall) {
        hitbox.x += move_intent.x;
        hitbox.y += move_intent.y;
    }

    // --- Schritt 4: ZEICHENPOSITION AKTUALISIEREN (DAS IST DIE ÄNDERUNG) ---
    // Die Zeichenposition ist jetzt die Hitbox-Position MINUS dem Offset.
    player_Pos.x = hitbox.x - hitbox_offset.x;
    player_Pos.y = hitbox.y - hitbox_offset.y;

    Update_Facing_Direction();
}

// Phase 3 :: Kollisionsreaktion
void Player_Base_Class::On_Collision(Collidable* other)
{
    Collision_Type otherType = other->Get_Collision_Type();

    // Die Logik für Wände ist jetzt in Tick().
    // Hier reagieren wir nur noch auf Interaktionen.
    if (otherType == Collision_Type::ENEMY)
    {
        // Hier kannst du Schaden nehmen, wenn du einen Gegner berührst.
        // Beispiel: Take_Damage(5);
    }

    // Hier kannst du später auf Projektile, Items etc. reagieren.
}

void Player_Base_Class::Draw()
{
    DrawTexture(this->maintex, this->hitbox.x,hitbox.y,WHITE);
}

void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown = 0.0f;
}

void Player_Base_Class::Ranged_Attack()
{
    Vector2 target_Position = game::core::Store::mouse_Position;
    float delta_vector_x = target_Position.x - this->hitbox.x;
    float delta_vector_y = target_Position.y - this->hitbox.y;
    float distance_to_target = std::sqrt(delta_vector_x * delta_vector_x + delta_vector_y * delta_vector_y);

    if (distance_to_target > 0) {
        Vector2 fire_direction = { delta_vector_x / distance_to_target, delta_vector_y / distance_to_target };
        sp_projectiles.push_back(std::make_unique<game::Player_Projectile>(
                Vector2{this->hitbox.x, this->hitbox.y},
                fire_direction,
                this->projectile_Speed,
                this->player_Damage,
                game::Config::player_Projectile_Sprite_Path
        ));
        ranged_Cooldown = 0.5f;
    }
}

void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = hitbox.x;
    previous_Position.y = hitbox.y;
}

void Player_Base_Class::Update_Facing_Direction()
{
    bool up = IsKeyDown(game::Config::key_Up);
    bool down = IsKeyDown(game::Config::key_Down);
    bool left = IsKeyDown(game::Config::key_Left);
    bool right = IsKeyDown(game::Config::key_Right);

    if ((up && down) || (left && right)) return;

    if (up && right) facing_Direction = Facing_Direction::UP_RIGHT;
    else if (up && left) facing_Direction = Facing_Direction::UP_LEFT;
    else if (down && right) facing_Direction = Facing_Direction::DOWN_RIGHT;
    else if (down && left) facing_Direction = Facing_Direction::DOWN_LEFT;
    else if (up) facing_Direction = Facing_Direction::UP;
    else if (down) facing_Direction = Facing_Direction::DOWN;
    else if (left) facing_Direction = Facing_Direction::LEFT;
    else if (right) facing_Direction = Facing_Direction::RIGHT;
}

Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}

Vector2 Player_Base_Class::Get_Player_Pos() {
    return this->player_Pos;
}

void Player_Base_Class::Take_Damage(int damage_amount)
{
    player_Health -= damage_amount;
}

Vector2 Player_Base_Class::Get_Player_Center()
{
    return (Vector2){player_Pos.x+maintex.width/2,player_Pos.y+maintex.height/2};
}