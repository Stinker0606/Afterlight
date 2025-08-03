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

void PlayerClass::Tick(float delta_time)
{
    // 1. Rufe die Tick-Methode der Basisklasse auf.
    // Diese kümmert sich um die Bewegung und setzt die `is_Moving` und `facing_Direction` Variablen.
    Player_Base_Class::Tick(delta_time);

    // 2. Aktualisiere unsere Timer
    if (hit_feedback_timer > 0.0f)
    {
        hit_feedback_timer -= delta_time;
        if (hit_feedback_timer <= 0.0f)
        {
            tint_color = WHITE; // Setze die Farbe zurück, wenn der Timer abgelaufen ist
        }
    }

    // 3. Unsere eigene Zustands-Logik die die Basisklasse erweitert
    // Nur wenn der Spieler nicht gerade eine andere Aktion ausführt...
    if (player_state != PlayerState::ATTACKING_RANGED && player_state != PlayerState::ATTACKING_MELEE)
    {
        // ...aktualisieren wir den Zustand basierend auf der Bewegung.
        player_state = is_Moving ? PlayerState::MOVING : PlayerState::IDLE;

        // Prüfe ob ein Angriff gestartet werden soll
        if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0.0f)
        {
            player_state = PlayerState::ATTACKING_RANGED;
            attack_animation_timer = game::Config::player_Ranged_Attack_Anim_Duration;
            ranged_Cooldown = game::Config::player_Ranged_Attack_Cooldown;
        }
    }
        else if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0.0f)
    	{
            player_state = PlayerState::ATTACKING_MELEE;
            attack_animation_timer = game::Config::player_Melee_Attack_Anim_Duration;
            melee_Cooldown = game::Config::player_Melee_Attack_Cooldown;
        }
    // Logik für den Fernkampf-Angriff
    else if (player_state == PlayerState::ATTACKING_RANGED)
    {
        attack_animation_timer -= delta_time;
        if (attack_animation_timer <= 0.0f)
        {
            Player_Base_Class::Ranged_Attack();
            player_state = PlayerState::IDLE;
        }
    }
    // Logik für den Nahkampf-Angriff
    else if (player_state == PlayerState::ATTACKING_MELEE)
    {
        attack_animation_timer -= delta_time;
        if (attack_animation_timer <= 0.0f)
        {
            Player_Base_Class::Melee_Attack();
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