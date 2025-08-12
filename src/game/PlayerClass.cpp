#include "PlayerClass.h"
#include "Store.h"
#include "../game/interactables/interact_list.h"

PlayerClass::PlayerClass(Vector2 start_Position, Object_Manager& om)
    // 1. Rufe den Konstruktor der Basisklasse mit den Werten aus der Config auf
    : Player_Base_Class(
        game::Config::player_Max_Health,
        game::Config::player_Movement_Speed,
        game::Config::player_Damage,
        start_Position,
        om
      ),
      // 2. Initialisiere alle Zustandsvariablen
      player_state(PlayerState::IDLE),
      attack_animation_timer(0.0f),
      hit_feedback_timer(0.0f),
      push_animation_timer(0.0f),
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
}

void PlayerClass::Set_Camera(std::shared_ptr<Cam> camera)
{
    this->sp_camera = camera;
}

// On_Collision Methode für spezielle Spieler-Interaktionen
void PlayerClass::On_Collision(std::shared_ptr<Collidable> other)
{
    // Reagiere nur auf neue Kollisionen, wenn du nicht gerade eine Aktion ausführst.
    if (player_state != PlayerState::IDLE && player_state != PlayerState::MOVING) return;

    if (auto push_block = std::dynamic_pointer_cast<Push_Block>(other))
    {
        // Wenn wir auf einen schiebbaren Block treffen, starten wir die PUSHING-Aktion.
        player_state = PlayerState::PUSHING;
        push_animation_timer = game::Config::player_Push_Anim_Duration;

        block_to_push = push_block;

        Vector2 move_dir = { hitbox.x - previous_Position.x, hitbox.y - previous_Position.y };
        if (fabs(move_dir.x) > fabs(move_dir.y)) {
            push_direction = { (move_dir.x > 0) ? 1.0f : -1.0f, 0.0f };
        } else {
            push_direction = { 0.0f, (move_dir.y > 0) ? 1.0f : -1.0f };
        }

        // Setze Spielerposition zurück und stoppe Bewegung
        hitbox.x = previous_Position.x;
        hitbox.y = previous_Position.y;
        player_Pos = previous_Position;
        is_Moving = false;
    }
    else
    {
        // Wenn es kein Push-Block ist, benutze die Standard-Kollisionslogik der Basisklasse.
        Player_Base_Class::On_Collision(other);
    }
}

// die Ranged_Attack Methode
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
            this->player_Damage,
            this->facing_Direction
        );
        // 1. Füge das Projektil dem Object_Manager hinzu, damit es gezeichnet
        //    und auf Kollisionen geprüft wird.
        om.AddObject(projectile);

        // 2. Füge das Projektil auch zur eigenen Liste des Spielers hinzu.
        //    Das "hält" den shared_ptr am Leben und verhindert, dass das Objekt sofort zerstört wird.
        sp_projectiles.push_back(projectile);

        ranged_Cooldown = game::Config::player_Ranged_Attack_Cooldown;
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
    // Bomben-Cooldown-Timer aktualisieren
    if (bomb_cooldown_ > 0.0f) {
        bomb_cooldown_ -= delta_time;
    }

    // Deine Zustands-Logik (angepasst an if/else if)
    if (player_state == PlayerState::IDLE || player_state == PlayerState::MOVING)
    {
        player_state = is_Moving ? PlayerState::MOVING : PlayerState::IDLE;

        if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_RANGED;
            attack_animation_timer = game::Config::player_Ranged_Attack_Anim_Duration;
        } else if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0.0f) {
            player_state = PlayerState::ATTACKING_MELEE;
            attack_animation_timer = game::Config::player_Melee_Attack_Anim_Duration;
            melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
        }  if (IsKeyPressed(game::Config::key_Place_Bomb) && bomb_count_ > 0 && bomb_cooldown_ <= 0.0f)
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
        if (attack_animation_timer <= 0.0f) {
            Player_Base_Class::Melee_Attack();
            player_state = PlayerState::IDLE;
        }
    }
    else if (player_state == PlayerState::PUSHING)
    {
        push_animation_timer -= delta_time;
        if (push_animation_timer <= 0.0f) {
            if (auto locked_block = block_to_push.lock()) {
                locked_block->Push(push_direction);
            }
            player_state = PlayerState::IDLE;
        }
    }
    /*
    else if (player_state == PlayerState::PUSHING)
    {
        push_animation_timer -= delta_time;
        if (push_animation_timer <= 0.0f) {
            if (auto locked_block = block_to_push.lock()) {
                // --- NEUE VORAUSSCHAUENDE PRÜFUNG ---
                // 1. Berechne, wo der Block nach dem Stoß sein würde.
                Rectangle future_hitbox = locked_block->Get_Hitbox();
                future_hitbox.x += push_direction.x * 32.0f;
                future_hitbox.y += push_direction.y * 32.0f;

                // 2. Prüfe, ob dieser zukünftige Platz frei ist.
                bool can_push = true;
                for (const auto& other_obj : om.managed_objects)
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
                // --- ENDE DER NEUEN LOGIK ---
            }
            player_state = PlayerState::IDLE; // Gehe immer in den Idle-Zustand, egal ob der Push erfolgreich war.
        }
    }
    */
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
                case Facing_Direction::UP:    p_current_animation = &anim_Sweep_Back; break;
                case Facing_Direction::DOWN:  p_current_animation = &anim_Sweep_Front; break;
                case Facing_Direction::LEFT:  p_current_animation = &anim_Sweep_Left; break;
                case Facing_Direction::RIGHT: p_current_animation = &anim_Sweep_Right; break;
                default:                      p_current_animation = &anim_Sweep_Front; break;
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
    // Erhöhe die Gesundheit, aber nicht über das Maximum
    this->player_Health += amount;
    if (this->player_Health > this->player_Max_Health)
    {
        this->player_Health = this->player_Max_Health;
    }
}

void PlayerClass::Take_Damage(int damage)
{
    this->player_Health -= damage;

    // LÖSE FEEDBACK NUR BEI SCHADEN AUS
    if (damage > 0) {
        hit_feedback_timer = 0.2f;
        tint_color = (Color){ 88, 60, 72, 255 };
    }
/*
    if (this->player_Health <= 0)
    {
        player_state = PlayerState::DYING;
    }
*/
}

void PlayerClass::Add_Key(int amount)
{
    this->key_count_ += amount;
}

int PlayerClass::Get_Key_Count() const
{
    return this->key_count_;
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