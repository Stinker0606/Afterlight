#pragma once

#include "PlayerBaseClass.h"
#include "../core/RepeatAnimation.h"
#include "../core/ControllableAnimations.h"
#include "../core/Cam.h"
#include <memory>
#include "PlayerProjectile.h"

class Push_Block;

/**
 * @brief spezifische, spielbare Charakter-Klasse.
 * Erbt die grundlegende Logik von PlayerBaseClass und erweitert sie
 * um eine komplexe Animations- und Zustandslogik.
 */
class PlayerClass : public Player_Base_Class
{
private:
    // Ein Enum für klare Spieler-Zustände.
    enum class PlayerState { IDLE, MOVING, ATTACKING_RANGED, ATTACKING_MELEE, PUSHING, DYING };
    PlayerState player_state;

    // Ein Zeiger der immer auf die gerade aktive Animation zeigt.
    void* p_current_animation;

    // Variable für den Punktestand
    int score_ = 0;

    // --- Timer für Zustände ---
    float hit_feedback_timer;       // Timer für die Dauer des roten Aufleuchtens -> not working under shader
    float hit_feedback_total_time;  // wie lange insgesamt noch Feedback läuft
    float push_cooldown_timer;      // Cooldown nach einer Schiebe-Aktion
    float walk_sound_timer_ = 0.0f; // Timer für den Lauf-Sound

    // --- PUSH WALL ---
    float push_animation_timer; // Eigener Timer für die Push-Animation
    std::weak_ptr<Push_Block> block_to_push; // Ein schwacher Zeiger auf den Block, den wir schieben
    Vector2 push_direction; // Die Richtung, in die wir schieben

    // --- Visuelle Effekte ---
    Color tint_color; // Die aktuelle Tönung des Spielers.

    // --- INVENTAR-ZÄHLER ---
    int key_count_ = 0;
    int bomb_count_ = 0;

    // --- BOMBEN ---
    bool should_place_bomb_ = false;
    float bomb_cooldown_ = 0.0f;

    // --- MELEE ---
    bool melee_hitbox_spawned_;

    // --- RANGED ---
    bool has_fired_projectile_;

    // --- Animationen ---
    void Update_Animation_Pointer(); // Eine Methode für die Animationsauswahl
    float facing_angle_; // Speichert den genauen Winkel zum Zielen.

    // Idle
    RepeatAnimation anim_Idle_Front;
    RepeatAnimation anim_Idle_Back;
    RepeatAnimation anim_Idle_Left;
    RepeatAnimation anim_Idle_Right;
    RepeatAnimation anim_Idle_Front_Right;
    RepeatAnimation anim_Idle_Back_Right;
    RepeatAnimation anim_Idle_Front_Left;
    RepeatAnimation anim_Idle_Back_Left;

    // Run
    RepeatAnimation anim_Run_Front;
    RepeatAnimation anim_Run_Back;
    RepeatAnimation anim_Run_Left;
    RepeatAnimation anim_Run_Right;
    RepeatAnimation anim_Run_Front_Right;
    RepeatAnimation anim_Run_Back_Right;
    RepeatAnimation anim_Run_Front_Left;
    RepeatAnimation anim_Run_Back_Left;

    // Attack Throw
    ControllableAnimations anim_Throw_Front;
    ControllableAnimations anim_Throw_Back;
    ControllableAnimations anim_Throw_Left;
    ControllableAnimations anim_Throw_Right;
    ControllableAnimations anim_Throw_Front_Right;
    ControllableAnimations anim_Throw_Front_Left;
    ControllableAnimations anim_Throw_Back_Left;
    ControllableAnimations anim_Throw_Back_Right;

    // Attack Sweep
    ControllableAnimations anim_Sweep_Front;
    ControllableAnimations anim_Sweep_Back;
    ControllableAnimations anim_Sweep_Left;
    ControllableAnimations anim_Sweep_Right;

    // Pushing
    ControllableAnimations anim_Push_Front;
    ControllableAnimations anim_Push_Back;
    ControllableAnimations anim_Push_Left;
    ControllableAnimations anim_Push_Right;

    // Dying
    ControllableAnimations anim_Dying;

    // Ein schwacher Zeiger auf die Kamera, um die Mausposition umzurechnen.
    std::weak_ptr<Cam> sp_camera;

    // Hält die ID der getragenen Waffe. -1 bedeutet, keine Waffe wird getragen.
    int held_weapon_id_ = -1;

    // Variablen hit Feedback
    int hit_feedback_blinks_left;
    bool hit_feedback_on;

public:
    /**
     * @brief Konstruktor für unseren Spieler.
     * @param start_Position Die Startposition in der Welt.
     * @param om Referenz zum Object_Manager, um Projektile zu spawnen.
     */
    PlayerClass(Vector2 start_Position, Object_Manager* om);
    ~PlayerClass() override = default;

    // Eine Methode, um den Kamera-Zeiger zu setzen.
    void Set_Camera(std::shared_ptr<Cam> camera);

    // Wir überschreiben die Ranged_Attack- sowie Melee_Attack-Methode der Basisklasse.
    void Melee_Attack() override;
    void Ranged_Attack() override;

    void On_Collision(std::shared_ptr<Collidable> other) override;

    /**
     * @brief Überschreibt die Tick-Methode der Basisklasse, um unsere Zustandslogik hinzuzufügen.
     * @param delta_time Die Zeit seit dem letzten Frame.
     */
    void Tick(float delta_time) override;

    /**
     * @brief die Draw-Methode, um die Animation zu zeichnen.
     */
    void Draw() override;

    /**
     * @brief Überschreibt die Take_Damage-Methode, um das Hit-Feedback auszulösen.
     * @param damage Die Höhe des Schadens.
     */
    void Take_Damage(int damage) override;

     /**
     * @brief Eine neue Heal-Methode um den Heal auszulösen.
     * @param amount Die Höhe des Heals.
     */
    void Heal(int amount);

    // --- METHODEN ZUM VERWALTEN DES INVENTARS ---
    void Add_Key(int amount);
    int Get_Key_Count() const;
    void Use_Key(int amount);
    void Add_Bomb(int amount);
    int Get_Bomb_Count() const;
    void Use_Bomb();
    bool Should_Place_Bomb();

    // --- METHODEN FÜR DEN SCORE ---
    void Add_Score(int amount);
    int Get_Score() const;

    // Methoden zum Verwalten der getragenen Waffe
    void SetHeldWeapon(int weapon_id);
    int GetHeldWeapon() const;
    void ClearHeldWeapon();
};
