#pragma once

#include "PlayerBaseClass.h"
#include "../core/RepeatAnimation.h"
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
    RepeatAnimation* p_current_animation;

    // --- Timer für Zustände ---
    float attack_animation_timer; // Timer für die Dauer von Angriffs-Animationen
    float hit_feedback_timer;     // Timer für die Dauer des roten Aufleuchtens

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

    // --- Animationen ---

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
    RepeatAnimation anim_Throw_Front;
    RepeatAnimation anim_Throw_Back;
    RepeatAnimation anim_Throw_Left;
    RepeatAnimation anim_Throw_Right;

    // Attack Sweep
    RepeatAnimation anim_Sweep_Front;
    RepeatAnimation anim_Sweep_Back;
    RepeatAnimation anim_Sweep_Left;
    RepeatAnimation anim_Sweep_Right;

    // Pushing
    RepeatAnimation anim_Push_Front;
    RepeatAnimation anim_Push_Back;
    RepeatAnimation anim_Push_Left;
    RepeatAnimation anim_Push_Right;

    // Dying
    RepeatAnimation anim_Dying;

    // Ein schwacher Zeiger auf die Kamera, um die Mausposition umzurechnen.
    std::weak_ptr<Cam> sp_camera;

public:
    /**
     * @brief Konstruktor für unseren Spieler.
     * @param start_Position Die Startposition in der Welt.
     * @param om Referenz zum Object_Manager, um Projektile zu spawnen.
     */
    PlayerClass(Vector2 start_Position, Object_Manager& om);
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
};
