#include "PlayerClass.h"
#include <iostream>
#include "Store.h"
#include "../scenes/LevelScene.h"
#include <string>
#include "../game/interactables/interact_list.h"
#include "interactables/MeleeHitbox.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

using namespace std::string_literals;

PlayerClass::PlayerClass(Vector2 start_Position, Object_Manager* om)
    // 1. Rufe den Konstruktor der Basisklasse mit den Werten aus der Config auf
    : Player_Base_Class(
        game::Config::player_Max_Health,
        game::Config::player_Movement_Speed,
        start_Position,
        om
      ),
      // 2. Initialisiere alle Zustandsvariablen
      player_state(PlayerState::IDLE),
      hit_feedback_timer(0.0f),
      push_animation_timer(0.0f),
      push_cooldown_timer(0.0f),
      push_direction({0,0}),
      tint_color(WHITE),

      // 3. Initialisiere ALLE Animationen mit den Werten aus der Config

      // Idle
      anim_Idle_Front(game::Config::player_animation_size, game::Config::player_Idle_Front_Path, game::Config::player_Idle_Front_Frames, game::Config::player_Idle_Front_Frames_Per_Line),
      anim_Idle_Back(game::Config::player_animation_size, game::Config::player_Idle_Back_Path, game::Config::player_Idle_Back_Frames, game::Config::player_Idle_Back_Frames_Per_Line),
      anim_Idle_Left(game::Config::player_animation_size, game::Config::player_Idle_Left_Path, game::Config::player_Idle_Left_Frames, game::Config::player_Idle_Left_Frames_Per_Line),
      anim_Idle_Right(game::Config::player_animation_size, game::Config::player_Idle_Right_Path, game::Config::player_Idle_Right_Frames, game::Config::player_Idle_Right_Frames_Per_Line),
      anim_Idle_Front_Right(game::Config::player_animation_size, game::Config::player_Idle_Front_Right_Path, game::Config::player_Idle_Front_Right_Frames, game::Config::player_Idle_Front_Right_Frames_Per_Line),
      anim_Idle_Back_Right(game::Config::player_animation_size, game::Config::player_Idle_Back_Right_Path, game::Config::player_Idle_Back_Right_Frames, game::Config::player_Idle_Back_Right_Frames_Per_Line),
      anim_Idle_Front_Left(game::Config::player_animation_size, game::Config::player_Idle_Front_Left_Path, game::Config::player_Idle_Front_Left_Frames, game::Config::player_Idle_Front_Left_Frames_Per_Line),
      anim_Idle_Back_Left(game::Config::player_animation_size, game::Config::player_Idle_Back_Left_Path, game::Config::player_Idle_Back_Left_Frames, game::Config::player_Idle_Back_Left_Frames_Per_Line),
      // Run
      anim_Run_Front(game::Config::player_animation_size, game::Config::player_Run_Front_Path, game::Config::player_Run_Front_Frames, game::Config::player_Run_Front_Frames_Per_Line),
      anim_Run_Back(game::Config::player_animation_size, game::Config::player_Run_Back_Path, game::Config::player_Run_Back_Frames, game::Config::player_Run_Back_Frames_Per_Line),
      anim_Run_Left(game::Config::player_animation_size, game::Config::player_Run_Left_Path, game::Config::player_Run_Left_Frames, game::Config::player_Run_Left_Frames_Per_Line),
      anim_Run_Right(game::Config::player_animation_size, game::Config::player_Run_Right_Path, game::Config::player_Run_Right_Frames, game::Config::player_Run_Right_Frames_Per_Line),
      anim_Run_Front_Right(game::Config::player_animation_size, game::Config::player_Run_Front_Right_Path, game::Config::player_Run_Front_Right_Frames, game::Config::player_Run_Front_Right_Frames_Per_Line),
      anim_Run_Back_Right(game::Config::player_animation_size, game::Config::player_Run_Back_Right_Path, game::Config::player_Run_Back_Right_Frames, game::Config::player_Run_Back_Right_Frames_Per_Line),
      anim_Run_Front_Left(game::Config::player_animation_size, game::Config::player_Run_Front_Left_Path, game::Config::player_Run_Front_Left_Frames, game::Config::player_Run_Front_Left_Frames_Per_Line),
      anim_Run_Back_Left(game::Config::player_animation_size, game::Config::player_Run_Back_Left_Path, game::Config::player_Run_Back_Left_Frames, game::Config::player_Run_Back_Left_Frames_Per_Line),
      // Attack Throw
      anim_Throw_Front(game::Config::player_animation_size, game::Config::player_Throw_Front_Path, game::Config::player_Throw_Front_Frames, game::Config::player_Throw_Front_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Back(game::Config::player_animation_size, game::Config::player_Throw_Back_Path, game::Config::player_Throw_Back_Frames, game::Config::player_Throw_Back_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Left(game::Config::player_animation_size, game::Config::player_Throw_Left_Path, game::Config::player_Throw_Left_Frames, game::Config::player_Throw_Left_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Right(game::Config::player_animation_size, game::Config::player_Throw_Right_Path, game::Config::player_Throw_Right_Frames, game::Config::player_Throw_Right_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Front_Right(game::Config::player_animation_size, game::Config::player_Throw_Front_Right_Path, game::Config::player_Throw_Front_Right_Frames, game::Config::player_Throw_Front_Right_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Front_Left(game::Config::player_animation_size, game::Config::player_Throw_Front_Left_Path, game::Config::player_Throw_Front_Left_Frames, game::Config::player_Throw_Front_Left_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Back_Left(game::Config::player_animation_size, game::Config::player_Throw_Back_Left_Path, game::Config::player_Throw_Back_Left_Frames, game::Config::player_Throw_Back_Left_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      anim_Throw_Back_Right(game::Config::player_animation_size, game::Config::player_Throw_Back_Right_Path, game::Config::player_Throw_Back_Right_Frames, game::Config::player_Throw_Back_Right_Frames_Per_Line, game::Config::player_Throw_Timings, false),
      // Attack Sweep
      anim_Sweep_Front(game::Config::player_animation_size, game::Config::player_Sweep_Front_Path, game::Config::player_Sweep_Front_Frames, game::Config::player_Sweep_Front_Frames_Per_Line, game::Config::player_Sweep_Front_Timings, false),
      anim_Sweep_Back(game::Config::player_animation_size, game::Config::player_Sweep_Back_Path, game::Config::player_Sweep_Back_Frames, game::Config::player_Sweep_Back_Frames_Per_Line, game::Config::player_Sweep_Back_Timings, false),
      anim_Sweep_Left(game::Config::player_animation_size, game::Config::player_Sweep_Left_Path, game::Config::player_Sweep_Left_Frames, game::Config::player_Sweep_Left_Frames_Per_Line, game::Config::player_Sweep_Left_Timings, false),
      anim_Sweep_Right(game::Config::player_animation_size, game::Config::player_Sweep_Right_Path, game::Config::player_Sweep_Right_Frames, game::Config::player_Sweep_Right_Frames_Per_Line, game::Config::player_Sweep_Right_Timings, false),
      // Pushing
      anim_Push_Front(game::Config::player_animation_size, game::Config::player_Push_Front_Path, game::Config::player_Push_Front_Frames, game::Config::player_Push_Front_Frames_Per_Line),
      anim_Push_Back(game::Config::player_animation_size, game::Config::player_Push_Back_Path, game::Config::player_Push_Back_Frames, game::Config::player_Push_Back_Frames_Per_Line),
      anim_Push_Left(game::Config::player_animation_size, game::Config::player_Push_Left_Path, game::Config::player_Push_Left_Frames, game::Config::player_Push_Left_Frames_Per_Line),
      anim_Push_Right(game::Config::player_animation_size, game::Config::player_Push_Right_Path, game::Config::player_Push_Right_Frames, game::Config::player_Push_Right_Frames_Per_Line),
      // Dying
      anim_Dying(game::Config::player_animation_size, game::Config::player_Dying_Path, game::Config::player_Dying_Frames, game::Config::player_Dying_Frames_Per_Line)
{
    // 4. Setze die Standard-Animation beim Start
    p_current_animation = &anim_Idle_Front;
    this->useFog = true;
    this->melee_hitbox_spawned_ = false;
    this->has_fired_projectile_ = false;
    this->walk_sound_timer_ = 0.0f;
    this->facing_angle_ = 0.0f;
}

void PlayerClass::Set_Camera(std::shared_ptr<Cam> camera)
{
    this->sp_camera = camera;
}

// On_Collision Methode für spezielle Spieler-Interaktionen
void PlayerClass::On_Collision(std::shared_ptr<Collidable> other)
{
    // Reagiere nur, wenn der Spieler nicht gerade eine andere Aktion ausführt.
    if (player_state != PlayerState::IDLE && player_state != PlayerState::MOVING) return;

    // --- PORTAL-LOGIK ---
    if (other->Get_Collision_Type() == Collision_Type::PORTAL)
    {
        if (auto door = std::dynamic_pointer_cast<Door>(other))
        {
            SoundManager::GetInstance().PlaySfx("door_transition");

            // 1. Schreibe die Zieldaten in den globalen Store.
            game::core::Store::next_scene_map = door->Get_Target_Map();
            game::core::Store::next_spawn_point = door->Get_Target_Spawn_Point();

            // 2. Ersetze die aktuelle Szene durch eine NEUE Instanz der Level1Scene.
            // Die neue Szene wird sich beim Starten die neuen Daten aus dem Store holen.
            game::core::Store::stage->ReplaceWithNewScene("gameplay"s, "gameplay"s, std::make_unique<game::scenes::Level1Scene>());
        }
        return;
    }

    // --- SPEZIELLE INTERAKTIONS-LOGIK ---

    // 1. Prüfe, ob es eine KeyWall ist UND ob wir sie öffnen können.
    if (auto key_wall = std::dynamic_pointer_cast<KeyWall>(other))
    {
        if (this->Get_Key_Count() > 0)
        {
            SoundManager::GetInstance().PlaySfx("keywall_open");

            this->Use_Key(1);
            // Definiere den Radius und markiere alle Wände in der Nähe zur Zerstörung.
            float search_radius = 72.0f;
            Vector2 origin_center = key_wall->Get_Hitbox_Center();

            for (const auto& obj_to_check : p_om_->managed_objects)
            {
                if (auto wall_to_destroy = std::dynamic_pointer_cast<KeyWall>(obj_to_check))
                {
                    if (Vector2Distance(origin_center, wall_to_destroy->Get_Hitbox_Center()) <= search_radius)
                    {
                        wall_to_destroy->Mark_For_Destruction();
                    }
                }
            }
        }
    }

    // --- PHYSISCHE KOLLISIONS-LOGIK ---

    // 1. Die Kollision mit einem Push_Block ist ein Sonderfall, da sie den Spieler-Zustand ändert.
    if (auto push_block = std::dynamic_pointer_cast<Push_Block>(other))
    {
        SoundManager::GetInstance().PlaySfx("player_push_block");

        player_state = PlayerState::PUSHING;
        push_animation_timer = game::Config::player_Push_Anim_Duration;
        block_to_push = push_block;
        Vector2 move_dir = { hitbox.x - previous_Position.x, hitbox.y - previous_Position.y };
        if (fabs(move_dir.x) > fabs(move_dir.y)) {
            push_direction = { (move_dir.x > 0) ? 1.0f : -1.0f, 0.0f };
        } else {
            push_direction = { 0.0f, (move_dir.y > 0) ? 1.0f : -1.0f };
        }
        hitbox.x = previous_Position.x;
        hitbox.y = previous_Position.y;
        player_Pos = previous_Position;
        is_Moving = false;
    }
    else
    {
        // 2. Für ALLE ANDEREN soliden Objekte wird die Standard-Kollisionslogik aus der Basisklasse aufgerufen.
        Player_Base_Class::On_Collision(other);
    }
}

void PlayerClass::Ranged_Attack()
{
    SoundManager::GetInstance().PlaySfx("player_throw");
    // Nur ausführen wenn die Kamera existiert
    if (auto cam_ptr = sp_camera.lock())
    {
        // 1. Hole Mausposition und rechne sie in Welt-Koordinaten um
        Vector2 mouse_screen_pos = game::core::Store::mouse_Position;
        Vector2 target_world_pos = GetScreenToWorld2D(mouse_screen_pos, cam_ptr->cam);

        // 2. Nutze die MITTE des Spielers als Startpunkt
        Vector2 player_center = this->Get_Player_Center();

        // 3. Berechne den Richtungsvektor vom Startpunkt zur Maus
        Vector2 fire_direction = Vector2Normalize({
            target_world_pos.x - player_center.x,
            target_world_pos.y - player_center.y
        });

        // Wir starten in der Mitte des Spielers...
        Vector2 projectile_start_pos = player_center;
        // ...und bewegen den Startpunkt dann ein Stück nach vorne in Schussrichtung.
        // 32.0f bedeutet, es spawnt ca. eine Kachel vor dem Spieler.
        float spawn_offset = 32.0f;
        projectile_start_pos.x += fire_direction.x * spawn_offset;
        projectile_start_pos.y += fire_direction.y * spawn_offset;

        // 4. Erstelle das Projektil
        auto projectile = std::make_shared<game::Player_Projectile>(
            projectile_start_pos,
            fire_direction,
            game::Config::player_Projectile_Damage,
            this->facing_Direction
        );
        // 1. Füge das Projektil dem Object_Manager hinzu, damit es gezeichnet
        //    und auf Kollisionen geprüft wird.
        p_om_->AddObject(projectile);

        // 2. Füge das Projektil auch zur eigenen Liste des Spielers hinzu.
        //    Das "hält" den shared_ptr am Leben und verhindert, dass das Objekt sofort zerstört wird.
        sp_projectiles.push_back(projectile);

        ranged_Cooldown = game::Config::player_Ranged_Attack_Cooldown;
    }
}

void PlayerClass::Melee_Attack()
{
    SoundManager::GetInstance().PlaySfx("player_sweep");
    float sweep_breite = 16.0f; // Die lange Seite des Angriffs
    float sweep_hoehe = 48.0f;  // Die kurze Seite des Angriffs
    float hitbox_width, hitbox_height;
    Vector2 hitbox_pos;

    Vector2 player_center = this->Get_Player_Center();
    float offset = 12.0f;

    switch (facing_Direction)
    {
        // VERTIKALE ANGRIFFE (tauschen)
        case Facing_Direction::UP:
        case Facing_Direction::DOWN:
            hitbox_width = sweep_hoehe;
            hitbox_height = sweep_breite;
            if (facing_Direction == Facing_Direction::UP) {
                hitbox_pos = { player_center.x - hitbox_width / 2, player_center.y - offset - hitbox_height };
            } else {
                hitbox_pos = { player_center.x - hitbox_width / 2, player_center.y + offset };
            }
            break;

        // HORIZONTALE ANGRIFFE (Standard)
        case Facing_Direction::LEFT:
        case Facing_Direction::RIGHT:
            hitbox_width = sweep_breite;
            hitbox_height = sweep_hoehe;
            if (facing_Direction == Facing_Direction::LEFT) {
                hitbox_pos = { player_center.x - offset - hitbox_width, player_center.y - hitbox_height / 2 };
            } else {
                hitbox_pos = { player_center.x + offset, player_center.y - hitbox_height / 2 };
            }
            break;

        // --- LOGIK FÜR DIAGONALE ANGRIFFE ---
        default:
        {
            // Wir benutzen eine quadratische Hitbox für einen besseren "Fächer"-Effekt.
            hitbox_width = 32.0f;
            hitbox_height = 32.0f;
            float diagonal_offset = -4.0f; // Wie weit die Box verschoben wird.

            if (facing_Direction == Facing_Direction::UP_RIGHT) {
                hitbox_pos = { player_center.x + diagonal_offset, player_center.y - diagonal_offset - hitbox_height };
            } else if (facing_Direction == Facing_Direction::DOWN_RIGHT) {
                hitbox_pos = { player_center.x + diagonal_offset, player_center.y + diagonal_offset };
            } else if (facing_Direction == Facing_Direction::UP_LEFT) {
                hitbox_pos = { player_center.x - diagonal_offset - hitbox_width, player_center.y - diagonal_offset - hitbox_height };
            } else { // DOWN_LEFT
                hitbox_pos = { player_center.x - diagonal_offset - hitbox_width, player_center.y + diagonal_offset };
            }
            break;
        }
    }

    auto sweep_hitbox = std::make_shared<MeleeHitbox>(
        Rectangle{ hitbox_pos.x, hitbox_pos.y, hitbox_width, hitbox_height },
        0.2f,
        game::Config::player_Melee_Damage,
        Collision_Type::PLAYER
    );

    p_om_->AddObject(sweep_hitbox);
}

// Methode zum Auswählen und Synchronisieren der Animation
void PlayerClass::Update_Animation_Pointer()
{
    void* old_animation_ptr = p_current_animation;
    ControllableAnimations* old_controllable = nullptr;

    // Speichere den Fortschritt der alten Animation, falls es eine ControllableAnimation war
    if (player_state == PlayerState::ATTACKING_RANGED || player_state == PlayerState::ATTACKING_MELEE) {
        old_controllable = static_cast<ControllableAnimations*>(old_animation_ptr);
    }

    // Wähle die neue Animation basierend auf Zustand und Richtung aus
    switch (player_state)
    {
        case PlayerState::ATTACKING_MELEE:
            // Wähle die Richtung basierend auf dem genauen Winkel
                if (facing_angle_ >= 45 && facing_angle_ < 135)        p_current_animation = &anim_Sweep_Front;
                else if (facing_angle_ >= 135 && facing_angle_ < 225)  p_current_animation = &anim_Sweep_Left;
                else if (facing_angle_ >= 225 && facing_angle_ < 315)  p_current_animation = &anim_Sweep_Back;
                else                                                   p_current_animation = &anim_Sweep_Right;
        break;

            case PlayerState::ATTACKING_RANGED:
                switch (facing_Direction) {
                    case Facing_Direction::UP:    		p_current_animation = &anim_Throw_Back; break;
                    case Facing_Direction::DOWN:  		p_current_animation = &anim_Throw_Front; break;
                    case Facing_Direction::LEFT:  		p_current_animation = &anim_Throw_Left; break;
                    case Facing_Direction::RIGHT: 		p_current_animation = &anim_Throw_Right; break;
                    case Facing_Direction::UP_RIGHT:    p_current_animation = &anim_Throw_Back_Right; break;
                    case Facing_Direction::UP_LEFT:     p_current_animation = &anim_Throw_Back_Left; break;
                    case Facing_Direction::DOWN_RIGHT:  p_current_animation = &anim_Throw_Front_Right; break;
                    case Facing_Direction::DOWN_LEFT:   p_current_animation = &anim_Throw_Front_Left; break;
                    default:                      		p_current_animation = &anim_Throw_Front; break;
                }
            break;

            case PlayerState::PUSHING:
                switch (facing_Direction) {
                    case Facing_Direction::UP:    p_current_animation = &anim_Push_Back; break;
                    case Facing_Direction::DOWN:  p_current_animation = &anim_Push_Front; break;
                    case Facing_Direction::LEFT:  p_current_animation = &anim_Push_Left; break;
                    case Facing_Direction::RIGHT: p_current_animation = &anim_Push_Right; break;
                    default:                      p_current_animation = &anim_Push_Front; break;
                }
            break;

            case PlayerState::MOVING:
                switch (facing_Direction) {
                    case Facing_Direction::UP:   		p_current_animation = &anim_Run_Back; break;
                    case Facing_Direction::DOWN: 		p_current_animation = &anim_Run_Front; break;
                    case Facing_Direction::LEFT: 		p_current_animation = &anim_Run_Left; break;
                    case Facing_Direction::RIGHT:	 	p_current_animation = &anim_Run_Right; break;
                    case Facing_Direction::UP_RIGHT:    p_current_animation = &anim_Run_Back_Right; break;
                    case Facing_Direction::DOWN_RIGHT:  p_current_animation = &anim_Run_Front_Right; break;
                    case Facing_Direction::UP_LEFT:  	p_current_animation = &anim_Run_Back_Left; break;
                    case Facing_Direction::DOWN_LEFT: 	p_current_animation = &anim_Run_Front_Left; break;
                    default:                      		p_current_animation = &anim_Run_Front; break;
                }
            break;

            case PlayerState::IDLE:
                default:
                    switch (facing_Direction) {
                        case Facing_Direction::UP:    		p_current_animation = &anim_Idle_Back; break;
                        case Facing_Direction::DOWN:  		p_current_animation = &anim_Idle_Front; break;
                        case Facing_Direction::LEFT:  		p_current_animation = &anim_Idle_Left; break;
                        case Facing_Direction::RIGHT: 		p_current_animation = &anim_Idle_Right; break;
                        case Facing_Direction::UP_RIGHT:    p_current_animation = &anim_Idle_Back_Right; break;
                        case Facing_Direction::DOWN_RIGHT:  p_current_animation = &anim_Idle_Front_Right; break;
                        case Facing_Direction::UP_LEFT:  	p_current_animation = &anim_Idle_Back_Left; break;
                        case Facing_Direction::DOWN_LEFT: 	p_current_animation = &anim_Idle_Front_Left; break;
                        default:                      		p_current_animation = &anim_Idle_Front; break;
                    }
            break;

            case PlayerState::DYING:
                p_current_animation = &anim_Dying;
            break;
        }
    // Synchronisiere den Frame-Fortschritt, wenn sich die Animation geändert hat
    if (p_current_animation != old_animation_ptr && old_controllable) {
        auto* new_controllable = static_cast<ControllableAnimations*>(p_current_animation);
        if (new_controllable) {
            new_controllable->SetProgress(old_controllable->Get_Current_Frame(), old_controllable->Get_Frame_Progress());
        }
    }
}

void PlayerClass::Tick(float delta_time)
{
    // Update der Blickrichtung zur Maus (nur wenn nicht in einer Aktion gesperrt)
    if (player_state != PlayerState::PUSHING)
    {
        if (auto cam_ptr = sp_camera.lock())
        {
            Vector2 mouse_screen_pos = game::core::Store::mouse_Position;
            Vector2 target_world_pos = GetScreenToWorld2D(mouse_screen_pos, cam_ptr->cam);
            Vector2 look_direction = Vector2Normalize({target_world_pos.x - this->Get_Player_Center().x, target_world_pos.y - this->Get_Player_Center().y});
            float angle = atan2(look_direction.y, look_direction.x) * (180.0f / PI);
            if (angle < 0) angle += 360;
            this->facing_angle_ = angle;

            if (angle >= 337.5 || angle < 22.5) facing_Direction = Facing_Direction::RIGHT;
            else if (angle >= 22.5 && angle < 67.5) facing_Direction = Facing_Direction::DOWN_RIGHT;
            else if (angle >= 67.5 && angle < 112.5) facing_Direction = Facing_Direction::DOWN;
            else if (angle >= 112.5 && angle < 157.5) facing_Direction = Facing_Direction::DOWN_LEFT;
            else if (angle >= 157.5 && angle < 202.5) facing_Direction = Facing_Direction::LEFT;
            else if (angle >= 202.5 && angle < 247.5) facing_Direction = Facing_Direction::UP_LEFT;
            else if (angle >= 247.5 && angle < 292.5) facing_Direction = Facing_Direction::UP;
            else if (angle >= 292.5 && angle < 337.5) facing_Direction = Facing_Direction::UP_RIGHT;
        }
    }

    // Update der Spielerbewegung (nur wenn nicht in einer Aktion gesperrt)
    if (player_state != PlayerState::PUSHING && player_state != PlayerState::ATTACKING_RANGED && player_state != PlayerState::ATTACKING_MELEE)
    {
        Player_Base_Class::Tick(delta_time);
    }

    // Update der Timer
    if (hit_feedback_timer > 0.0f)
    {
        hit_feedback_timer -= delta_time;
        if (hit_feedback_timer <= 0.0f) { tint_color = WHITE; }
    }
    // Bomben-Cooldown-Timer
    if (bomb_cooldown_ > 0.0f) {
        bomb_cooldown_ -= delta_time;
    }
    // Walk-Sound-Timer
    if (walk_sound_timer_ > 0.0f) {
        walk_sound_timer_ -= delta_time;
    }

    // --- ZUSTANDS-LOGIK ---
    if (player_state == PlayerState::IDLE || player_state == PlayerState::MOVING)
    {
        player_state = is_Moving ? PlayerState::MOVING : PlayerState::IDLE;

        // --- FINALE LOGIK FÜR DEN LAUF-SOUND ---
        if (is_Moving)
        {
            // Wenn der Spieler sich bewegt, spiele den Sound rhythmisch ab.
            // Der Timer sorgt für den Abstand zwischen den Schritten.
            if (walk_sound_timer_ <= 0.0f) {
                SoundManager::GetInstance().PlaySfx("player_walk", 1);
                walk_sound_timer_ = game::AudioConfig::kWalk_Sound_Timer;
            }
        }
        else
        {
            // Wenn der Spieler STEHT, stoppe den Lauf-Sound sofort.
            SoundManager::GetInstance().StopSfx("player_walk");
        }

        // Prüfe auf Spieler-Aktionen
        if (IsMouseButtonPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_RANGED;
            has_fired_projectile_ = false; // Setze die Kontrolle vor jedem Wurf zurück.
            Update_Animation_Pointer(); // Wähle die Start-Animation
            static_cast<ControllableAnimations*>(p_current_animation)->Reset(); // Setze sie zurück
        } else if (IsMouseButtonPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_MELEE;
            melee_hitbox_spawned_ = false; // Setze die Spawn-Kontrolle zurück
            melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
            Update_Animation_Pointer(); // Wähle die Start-Animation
            static_cast<ControllableAnimations*>(p_current_animation)->Reset(); // Setze sie zurück
        }
        if (IsKeyPressed(game::Config::key_Place_Bomb) && bomb_count_ > 0 && bomb_cooldown_ <= 0.0f)
        {
            Use_Bomb();
        }
    }
    else if (player_state == PlayerState::ATTACKING_RANGED)
    {
        ControllableAnimations* current = static_cast<ControllableAnimations*>(p_current_animation);
        if (current && current->Get_Current_Frame() == 40) {
            this->Ranged_Attack();
        }
        if (current && current->Is_Finished()) {
            player_state = PlayerState::IDLE;
        }
    }
    else if (player_state == PlayerState::ATTACKING_MELEE) {
        ControllableAnimations* current = static_cast<ControllableAnimations*>(p_current_animation);
        // Die Hitbox erscheint bei Frame 4.
        if (current && current->Get_Current_Frame() == 4 && !melee_hitbox_spawned_) {
            this->Melee_Attack();
            melee_hitbox_spawned_ = true;
        }
        if (current && current->Is_Finished()) {
            player_state = PlayerState::IDLE;
        }
    }
    else if (player_state == PlayerState::PUSHING)
    {
        push_animation_timer -= delta_time;
        if (push_animation_timer <= 0.0f) {
            if (auto locked_block = block_to_push.lock()) {
                // 1. Berechne, wo der Block nach dem Stoß sein würde.
                Rectangle future_hitbox = locked_block->Get_Hitbox();
                future_hitbox.x += push_direction.x * 16.0f;
                future_hitbox.y += push_direction.y * 16.0f;

                // 2. Prüfe, ob dieser zukünftige Platz frei ist.
                bool can_push = true;
                //  Benutze den korrekten Member-Pointer 'p_om_'
                for (const auto& other_obj : p_om_->managed_objects)
                {
                    // Ignoriere den Block selbst und den Spieler
                    if (other_obj == locked_block || other_obj == shared_from_this()) continue;

                    Collision_Type type = other_obj->Get_Collision_Type();
                    if (type == Collision_Type::WALL || type == Collision_Type::ENEMY || type == Collision_Type::ENEMY_SPAWNER)
                    {
                        if (CheckCollisionRecs(future_hitbox, other_obj->Get_Hitbox()))
                        {
                            can_push = false; // Ein Hindernis ist im Weg!
                            break;
                        }
                    }
                }

                // 3. Nur wenn der Weg frei ist, bewege den Block.
                if (can_push) {
                    locked_block->Push(push_direction);
                }
            }
            player_state = PlayerState::IDLE; // Gehe immer in den Idle-Zustand, egal ob der Push erfolgreich war.
        }
    }

    else if (player_state == PlayerState::DYING)
    {
        // Hier kommt die Logik für den Tod hinein.
        // Z.B. Animation abspielen, nach einer Weile zum Game-Over-Screen wechseln.
        // Vorerst bleibt der Spieler einfach im DYING-Zustand.
    }
    Update_Animation_Pointer();
}

void PlayerClass::Draw()
{
        // 1. Zeichne den aktuellen Frame.
    if (p_current_animation)
    {
        Vector2 hitbox_position = Get_Player_Pos();
        Vector2 draw_position = {
            hitbox_position.x + game::Config::player_visual_offset.x,
            hitbox_position.y + game::Config::player_visual_offset.y
        };

        // Je nach Zustand casten wir den Pointer und rufen die richtige Draw-Funktion auf
        if (player_state == PlayerState::ATTACKING_MELEE || player_state == PlayerState::ATTACKING_RANGED) {
            auto anim = static_cast<ControllableAnimations*>(p_current_animation);
            anim->Draw_Current_Frame(draw_position, tint_color);
            // Rufe Next_Frame nur auf, wenn die Animation noch nicht beendet ist.
            if (!anim->Is_Finished()) {
                anim->Next_Frame();
            }
        } else {
            auto anim = static_cast<RepeatAnimation*>(p_current_animation);
            anim->Draw_Current_Frame(draw_position, tint_color);
            anim->Next_Frame();
        }
    }
}


void PlayerClass::Heal(int amount)
{
    // Verhindere Heilung, wenn der Spieler bereits tot ist.
    if (this->player_Health <= 0) return;

    // Erhöhe die Gesundheit, aber nicht über das Maximum.
    this->player_Health += amount;
    if (this->player_Health > this->player_Max_Health)
    {
        this->player_Health = this->player_Max_Health;
    }
}

void PlayerClass::Take_Damage(int damage)
{
    // Verhindere weiteren Schaden, wenn der Spieler bereits tot ist.
    if (this->player_Health <= 0) return;

    this->player_Health -= damage;

    // LÖSE FEEDBACK NUR BEI SCHADEN AUS
    if (damage > 0) {
        SoundManager::GetInstance().PlaySfx("player_hit");
        hit_feedback_timer = 0.2f;
        tint_color = (Color){ 88, 60, 72, 255 };
    }

    // Stelle sicher, dass die HP nicht unter 0 fallen.
    if (this->player_Health < 0) {
        SoundManager::GetInstance().PlaySfx("player_death");
        this->player_Health = 0;
        // Hier könntest du später den DYING-Zustand auslösen
        // player_state = PlayerState::DYING;
    }
}

void PlayerClass::Add_Key(int amount)
{
    this->key_count_ += amount;
}

int PlayerClass::Get_Key_Count() const
{
    return this->key_count_;
}

void PlayerClass::Use_Key(int amount)
{
    this->key_count_ -= amount;
    if (this->key_count_ < 0) {
        this->key_count_ = 0;
    }
}

void PlayerClass::Add_Bomb(int amount)
{
    this->bomb_count_ += amount;
}

int PlayerClass::Get_Bomb_Count() const
{
    return this->bomb_count_;
}

void PlayerClass::Use_Bomb()
{
    SoundManager::GetInstance().PlaySfx("player_place_bomb");
    bomb_count_--;
    should_place_bomb_ = true; // Signal für die Szene setzen
    bomb_cooldown_ = game::Config::kBombPlacementCooldown; // Cooldown zurücksetzen
}

bool PlayerClass::Should_Place_Bomb()
{
    if (should_place_bomb_) {
        should_place_bomb_ = false; // Signal zurücksetzen
        return true;
    }
    return false;
}

void PlayerClass::Add_Score(int amount)
{
    this->score_ += amount;
}

int PlayerClass::Get_Score() const
{
    return this->score_;
}