#include "PlayerClass.h"
#include <iostream>
#include "Store.h"
#include "../scenes/LevelScene.h"
#include <string>
#include "../game/interactables/interact_list.h"
#include "interactables/MeleeHitbox.h"

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
      attack_animation_timer(0.0f),
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
      anim_Throw_Front(game::Config::player_animation_size, game::Config::player_Throw_Front_Path, game::Config::player_Throw_Front_Frames, game::Config::player_Throw_Front_Frames_Per_Line),
      anim_Throw_Back(game::Config::player_animation_size, game::Config::player_Throw_Back_Path, game::Config::player_Throw_Back_Frames, game::Config::player_Throw_Back_Frames_Per_Line),
      anim_Throw_Left(game::Config::player_animation_size, game::Config::player_Throw_Left_Path, game::Config::player_Throw_Left_Frames, game::Config::player_Throw_Left_Frames_Per_Line),
      anim_Throw_Right(game::Config::player_animation_size, game::Config::player_Throw_Right_Path, game::Config::player_Throw_Right_Frames, game::Config::player_Throw_Right_Frames_Per_Line),
      anim_Throw_Front_Right(game::Config::player_animation_size, game::Config::player_Throw_Front_Right_Path, game::Config::player_Throw_Front_Right_Frames, game::Config::player_Throw_Front_Right_Frames_Per_Line),
      anim_Throw_Front_Left(game::Config::player_animation_size, game::Config::player_Throw_Front_Left_Path, game::Config::player_Throw_Front_Left_Frames, game::Config::player_Throw_Front_Left_Frames_Per_Line),
      anim_Throw_Back_Left(game::Config::player_animation_size, game::Config::player_Throw_Back_Left_Path, game::Config::player_Throw_Back_Left_Frames, game::Config::player_Throw_Back_Left_Frames_Per_Line),
      anim_Throw_Back_Right(game::Config::player_animation_size, game::Config::player_Throw_Back_Right_Path, game::Config::player_Throw_Back_Right_Frames, game::Config::player_Throw_Back_Right_Frames_Per_Line),
      // Attack Sweep
      anim_Sweep_Front(game::Config::player_animation_size, game::Config::player_Sweep_Front_Path, game::Config::player_Sweep_Front_Frames, game::Config::player_Sweep_Front_Frames_Per_Line),
      anim_Sweep_Back(game::Config::player_animation_size, game::Config::player_Sweep_Back_Path, game::Config::player_Sweep_Back_Frames, game::Config::player_Sweep_Back_Frames_Per_Line),
      anim_Sweep_Left(game::Config::player_animation_size, game::Config::player_Sweep_Left_Path, game::Config::player_Sweep_Left_Frames, game::Config::player_Sweep_Left_Frames_Per_Line),
      anim_Sweep_Right(game::Config::player_animation_size, game::Config::player_Sweep_Right_Path, game::Config::player_Sweep_Right_Frames, game::Config::player_Sweep_Right_Frames_Per_Line),
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

     if (auto push_block = std::dynamic_pointer_cast<Push_Block>(other))
    {
        // REGEL 0: Wenn der Cooldown aktiv ist, ist der Block eine normale Wand. Ende.
        if (push_cooldown_timer > 0.0f)
        {
            Player_Base_Class::On_Collision(other);
            return;
        }

        // REGEL 1: BESTIMME DIE BEWEGUNGSACHSE DES SPIELERS (Horizontal oder Vertikal).
        // Dies verhindert den Diagonal-Exploit.
        Vector2 move_vector = { hitbox.x - previous_Position.x, hitbox.y - previous_Position.y };
        bool is_moving_horizontally = fabs(move_vector.x) > fabs(move_vector.y);

        Rectangle player_hitbox = Get_Hitbox();
        Rectangle block_hitbox = push_block->Get_Hitbox();
        bool is_aligned = false;

        // REGEL 2: PRÜFE DIE AUSRICHTUNG DES SPIELERS ZUM BLOCK.
        // Ein "Anstupsen" mit der Ecke ist nicht mehr möglich.
        if (is_moving_horizontally)
        {
            // Spieler will horizontal schieben -> muss vertikal ausgerichtet sein.
            float y_overlap = std::min(player_hitbox.y + player_hitbox.height, block_hitbox.y + block_hitbox.height) - std::max(player_hitbox.y, block_hitbox.y);
            if (y_overlap > block_hitbox.height * 0.7f) // Mindestens 70% der BLOCK-Höhe müssen berührt werden.
            {
                is_aligned = true;
                push_direction = { (move_vector.x > 0) ? 1.0f : -1.0f, 0.0f };
            }
        }
        else // Spieler bewegt sich primär vertikal
        {
            // Spieler will vertikal schieben -> muss horizontal ausgerichtet sein.
            float x_overlap = std::min(player_hitbox.x + player_hitbox.width, block_hitbox.x + block_hitbox.width) - std::max(player_hitbox.x, block_hitbox.x);
            if (x_overlap > block_hitbox.width * 0.7f) // Mindestens 70% der BLOCK-Breite müssen berührt werden.
            {
                is_aligned = true;
                push_direction = { 0.0f, (move_vector.y > 0) ? 1.0f : -1.0f };
            }
        }

        // Wenn die Ausrichtung nicht stimmt, ist es eine normale Wand. Ende.
        if (!is_aligned)
        {
            Player_Base_Class::On_Collision(other);
            return;
        }

        // REGEL 3: PRÜFE, OB DER ZIELPLATZ FREI IST.
        // Das ist die finale Prüfung gegen das Durchschieben.
        Rectangle future_hitbox = push_block->Get_Hitbox();
        future_hitbox.x += push_direction.x * 32.0f;
        future_hitbox.y += push_direction.y * 32.0f;

        bool path_is_blocked = false;
        for (const auto& other_obj : p_om_->managed_objects)
        {
            if (other_obj == push_block || other_obj == shared_from_this()) continue;

            // Prüfe gegen JEDES solide Objekt (dünne Wände, andere Blöcke etc.)
            if (other_obj->Get_Collision_Type() == Collision_Type::WALL)
            {
                if (CheckCollisionRecs(future_hitbox, other_obj->Get_Hitbox()))
                {
                    path_is_blocked = true;
                    break;
                }
            }
        }

        // REGEL 4: FINALE AKTION
        if (!path_is_blocked)
        {
            // ALLES PASST: Starte die Schiebe-Aktion
            player_state = PlayerState::PUSHING;
            push_animation_timer = game::Config::player_Push_Anim_Duration;
            push_cooldown_timer = 0.3f; // Cooldown sofort aktivieren
            block_to_push = push_block;

            // Spieler zurücksetzen, um "Kleben" oder Durchrutschen zu verhindern
            hitbox.x = previous_Position.x;
            hitbox.y = previous_Position.y;
            player_Pos = previous_Position;
            is_Moving = false;
        }
        else
        {
            // Weg ist blockiert -> behandle als normale, unbewegliche Wand.
            Player_Base_Class::On_Collision(other);
        }
    }
    else
    {
        // Für alle anderen soliden Objekte, nutze die Standard-Kollisionslogik.
        Player_Base_Class::On_Collision(other);
    }
}

void PlayerClass::Ranged_Attack()
{
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

    // Hit-Feedback-Cooldown-Timer aktualisieren
    if (hit_feedback_timer > 0.0f)
    {
        hit_feedback_timer -= delta_time;
        if (hit_feedback_timer <= 0.0f) { tint_color = WHITE; }
    }

    // Push-Cooldown-Timer aktualisieren
    if (push_cooldown_timer > 0.0f) {
        push_cooldown_timer -= delta_time;
    }

    // Bomben-Cooldown-Timer aktualisieren
    if (bomb_cooldown_ > 0.0f) {
        bomb_cooldown_ -= delta_time;
    }

    // Deine Zustands-Logik (angepasst an if/else if)
    if (player_state == PlayerState::IDLE || player_state == PlayerState::MOVING)
    {
        player_state = is_Moving ? PlayerState::MOVING : PlayerState::IDLE;

        if (IsMouseButtonPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_RANGED;
            attack_animation_timer = game::Config::player_Ranged_Attack_Anim_Duration;
        } else if (IsMouseButtonPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_MELEE;
            attack_animation_timer = game::Config::player_Melee_Attack_Anim_Duration;
            melee_hitbox_spawned_ = false; // Setze die Spawn-Kontrolle zurück
            melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
        }
        if (IsKeyPressed(game::Config::key_Place_Bomb) && bomb_count_ > 0 && bomb_cooldown_ <= 0.0f)
        {
            Use_Bomb();
        }
    }
    else if (player_state == PlayerState::ATTACKING_RANGED)
    {
        attack_animation_timer -= delta_time;
        if (attack_animation_timer <= 0.0f) {
            this->Ranged_Attack();
            player_state = PlayerState::IDLE;
        }
    }
    else if (player_state == PlayerState::ATTACKING_MELEE)
    {
        attack_animation_timer -= delta_time;

        // Definiere den Zeitpunkt, wann die Hitbox erscheinen soll (Gesamtdauer - 0.2s)
        float spawn_time = game::Config::player_Melee_Attack_Anim_Duration - 0.2f;

        // Wenn der Zeitpunkt erreicht ist UND die Hitbox noch nicht erstellt wurde...
        if (attack_animation_timer <= spawn_time && !melee_hitbox_spawned_)
        {
            this->Melee_Attack(); // ...erstelle die Hitbox.
            melee_hitbox_spawned_ = true; // Markiere sie als erstellt.
        }

        // Wenn die Animation komplett vorbei ist, gehe zurück zum Stillstand.
        if (attack_animation_timer <= 0.0f) {
            player_state = PlayerState::IDLE;
        }
    }

    else if (player_state == PlayerState::PUSHING)
    {
        push_animation_timer -= delta_time;
        if (push_animation_timer <= 0.0f) {
            // Die Prüfung hat bereits stattgefunden. Wir führen den Push jetzt einfach aus.
            if (auto locked_block = block_to_push.lock()) {
                locked_block->Push(push_direction);
            }
            player_state = PlayerState::IDLE;
        }
    }

    else if (player_state == PlayerState::DYING)
    {
        // Hier kommt die Logik für den Tod hinein.
        // Z.B. Animation abspielen, nach einer Weile zum Game-Over-Screen wechseln.
        // Vorerst bleibt der Spieler einfach im DYING-Zustand.
    }
}


void PlayerClass::Draw()
{
	switch (player_state)
	{
    // 1. Wähle die korrekte Animation basierend auf unserem Zustand.
    case PlayerState::ATTACKING_RANGED:
            switch (facing_Direction) {
                case Facing_Direction::UP:    		p_current_animation = &anim_Throw_Back; break;
                case Facing_Direction::DOWN:  		p_current_animation = &anim_Throw_Front; break;
                case Facing_Direction::LEFT:  		p_current_animation = &anim_Throw_Left; break;
                case Facing_Direction::RIGHT: 		p_current_animation = &anim_Throw_Right; break;
                default:                      		p_current_animation = &anim_Throw_Front; break;
            }
            break;

        case PlayerState::ATTACKING_MELEE:
            switch (facing_Direction) {
                case Facing_Direction::UP:          p_current_animation = &anim_Sweep_Back; break;
                case Facing_Direction::DOWN:        p_current_animation = &anim_Sweep_Front; break;
                case Facing_Direction::LEFT:        p_current_animation = &anim_Sweep_Left; break;
                case Facing_Direction::RIGHT:       p_current_animation = &anim_Sweep_Right; break;
                case Facing_Direction::UP_RIGHT:    p_current_animation = &anim_Throw_Back_Right; break;
                case Facing_Direction::UP_LEFT:     p_current_animation = &anim_Throw_Back_Left; break;
                case Facing_Direction::DOWN_RIGHT:  p_current_animation = &anim_Throw_Front_Right; break;
                case Facing_Direction::DOWN_LEFT:   p_current_animation = &anim_Throw_Front_Left; break;
                default:                            p_current_animation = &anim_Sweep_Front; break;
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

    // 2. Zeichne den aktuellen Frame mit der Offset-Berechnung.
    if (p_current_animation)
    {
        // a) Hole die obere linke Ecke der Hitbox.
        Vector2 hitbox_position = Get_Player_Pos();

        // b) Addiere den visuellen Offset aus der Config.
        Vector2 draw_position = {
            hitbox_position.x + game::Config::player_visual_offset.x,
            hitbox_position.y + game::Config::player_visual_offset.y
        };

        // c) Zeichne die Animation an der Position.
        p_current_animation->Draw_Current_Frame(draw_position, tint_color);
        p_current_animation->Next_Frame();
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
        hit_feedback_timer = 0.2f;
        tint_color = (Color){ 88, 60, 72, 255 };
    }

    // Stelle sicher, dass die HP nicht unter 0 fallen.
    if (this->player_Health < 0) {
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