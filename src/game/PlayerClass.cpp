#include "PlayerClass.h"
#include "Store.h" // Für die Mausposition

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
}

void PlayerClass::Set_Camera(std::shared_ptr<Cam> camera)
{
    this->sp_camera = camera;
}

// die Ranged_Attack Methode
void PlayerClass::Ranged_Attack()
{
    // Nur ausführen wenn die Kamera existiert
    if (auto cam_ptr = sp_camera.lock())
    {
        // 1. Hole die Mausposition vom Bildschirm und rechne sie in Welt-Koordinaten um.
        Vector2 mouse_screen_pos = game::core::Store::mouse_Position;
        Vector2 target_world_pos = GetScreenToWorld2D(mouse_screen_pos, cam_ptr->cam);

        // --- Spwawn Point des Projectiles ---
        Vector2 player_center = this->Get_Player_Center();
        Vector2 spawn_offset = {0.0f, 0.0f};
        float offset_distance = 24.0f; // Wie weit vor dem Spieler das Projektil spawnen soll (halbe Kachel)

        // Bestimme den Offset basierend auf der aktuellen Blickrichtung des Spielers
        switch (this->facing_Direction)
        {
            case Facing_Direction::UP:          spawn_offset.y = -offset_distance; break;
            case Facing_Direction::DOWN:        spawn_offset.y = offset_distance;  break;
            case Facing_Direction::LEFT:        spawn_offset.x = -offset_distance; break;
            case Facing_Direction::RIGHT:       spawn_offset.x = offset_distance;  break;
            case Facing_Direction::UP_LEFT:     spawn_offset = Vector2Normalize({-1, -1}) * offset_distance; break;
            case Facing_Direction::UP_RIGHT:    spawn_offset = Vector2Normalize({1, -1}) * offset_distance;  break;
            case Facing_Direction::DOWN_LEFT:   spawn_offset = Vector2Normalize({-1, 1}) * offset_distance; break;
            case Facing_Direction::DOWN_RIGHT:  spawn_offset = Vector2Normalize({1, 1}) * offset_distance;  break;
        }

        Vector2 projectile_start_pos = { player_center.x + spawn_offset.x, player_center.y + spawn_offset.y };
        // ------------------------------------

        // 2. Berechne den Richtungsvektor vom NEUEN Startpunkt zur Maus
        Vector2 fire_direction = Vector2Normalize({
            target_world_pos.x - projectile_start_pos.x,
            target_world_pos.y - projectile_start_pos.y
        });

        // 3. Erstelle das Projektil am neuen Startpunkt
        auto projectile = std::make_shared<game::Player_Projectile>(
            projectile_start_pos,
            fire_direction,
            this->player_Damage,
            game::Config::player_Projectile_Sprite_Path
        );
        om.AddObject(projectile);

        // Setze den Cooldown zurück
        ranged_Cooldown = game::Config::player_Ranged_Attack_Cooldown;
    }
}

void PlayerClass::Tick(float delta_time)
{
    // --- Spieler-Ausrichtung zur Maus ---
    // Dieser Block wird zuerst ausgeführt, um die Blickrichtung für diesen Frame festzulegen.
    if (auto cam_ptr = sp_camera.lock())
    {
        Vector2 mouse_screen_pos = game::core::Store::mouse_Position;
        Vector2 target_world_pos = GetScreenToWorld2D(mouse_screen_pos, cam_ptr->cam);
        Vector2 look_direction = Vector2Normalize({
            target_world_pos.x - this->Get_Player_Center().x,
            target_world_pos.y - this->Get_Player_Center().y
        });

        // Konvertiere den Richtungsvektor in eine unserer 8 Facing_Directions
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
    // -----------------------------------------

    // 1. Rufe die Tick-Methode der Basisklasse auf.
    // Diese kümmert sich NUR NOCH um die Bewegung basierend auf den Tasten.
    Player_Base_Class::Tick(delta_time);

    // 2. Aktualisiere unsere Timer
    if (hit_feedback_timer > 0.0f)
    {
        hit_feedback_timer -= delta_time;
        if (hit_feedback_timer <= 0.0f)
        {
            tint_color = WHITE;
        }
    }

    // 3. Deine Zustands-Logik
    if (player_state != PlayerState::ATTACKING_RANGED && player_state != PlayerState::ATTACKING_MELEE)
    {
        player_state = is_Moving ? PlayerState::MOVING : PlayerState::IDLE;

        if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0.0f)
        {
            player_state = PlayerState::ATTACKING_RANGED;
            attack_animation_timer = game::Config::player_Ranged_Attack_Anim_Duration;
            ranged_Cooldown = game::Config::player_Ranged_Attack_Cooldown;
        }
        else if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0.0f)
        {
            player_state = PlayerState::ATTACKING_MELEE;
            attack_animation_timer = game::Config::player_Melee_Attack_Anim_Duration;
            melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
        }
    }
    // Logik für den Fernkampf-Angriff
    else if (player_state == PlayerState::ATTACKING_RANGED)
    {
        attack_animation_timer -= delta_time;
        if (attack_animation_timer <= 0.0f)
        {
            // Rufe die Ranged_Attack-Methode DIESER Klasse auf, nicht die der Basisklasse.
            this->Ranged_Attack();
            player_state = PlayerState::IDLE;
        }
    }
    // Logik für den Nahkampf-Angriff
    else if (player_state == PlayerState::ATTACKING_MELEE)
    {
        attack_animation_timer -= delta_time;
        if (attack_animation_timer <= 0.0f)
        {
            Player_Base_Class::Melee_Attack(); // Hier bleibt es vorerst bei der Basisklasse
            player_state = PlayerState::IDLE;
        }
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



void PlayerClass::Take_Damage(int damage)
{
    // 1. Rufe die Logik der Basisklasse auf, um die HP zu reduzieren.
    this->player_Health -= damage;

    // 2. Starte unser visuelles Hit-Feedback.
    hit_feedback_timer = 0.2f; // Für 0.2 Sekunden aufleuchten
    tint_color = (Color){ 88, 60, 72, 255 }; // Hex-Code #583c48
}