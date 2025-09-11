#pragma once

#include "Scene.h"
#include "LevelScreen.h"
#include "Object_Manager.h"
#include "CollisionManager.h"
#include "Cam.h"
#include <memory>
#include <vector>
#include "FogManager.h"
#include "UIManager.h"
#include "DialogManager.h"
#include "../core/DeltaTimeMachine.h"
#include "../game/PlayerClass.h"
#include "../game/spawner/Level1Spawner.h"
#include "../game/interactables/Statue.h"
#include "../game/interactables/KeyConsumable.h"

namespace game::scenes
{
    /**
     * @brief Dies ist DEINE Szene für Level 1.
     * Sie erbt von der Basis-Szene und enthält die gesamte Spiellogik,
     * die nur für diesen Level relevant ist. Sie ist unser sauberer Arbeitsbereich.
     */
    class Level1Scene final : public game::core::Scene
    {
    private:
        // --- Warteliste ---
        std::vector<std::shared_ptr<Collidable>> objects_to_add_list_;

        // --- Engine-Systeme ---
        Object_Manager objectManager;
        DT::timemachine dtm;
        std::unique_ptr<Collision_Manager> p_cm;

        // --- Spiel-Objekte ---
        std::weak_ptr<PlayerClass> sp_player;
        std::shared_ptr<Cam> sp_cam;

        // --- UI-Manager ---
        UIManager uiManager_;

        // --- Fog-Manager ---
        FogManager fogManager;

        // --- Dialog-Manager ---
        DialogManager dialogManager_;

        // Eine RenderTexture, die als unsere "Nebel-Maske" dient.
        RenderTexture2D fogMaskTexture;

        // --- Level-spezifische Daten ---
        int level_Nbr = 1;
        LevelScreen levelScreen{&level_Nbr}; // Wir benutzen unsere erweiterbare LevelScreen

        // Listen, die der EnemySpawner aus der Basis-Engine benötigt
        // Eine Liste, die alle Hindernisse für die Spawner enthält.
        std::vector<Rectangle> obstacle_list_for_spawner;

        // Eine temporäre Liste, in der der Spawner seine neu erstellten Gegner ablegt.
        std::vector<enemy::Enemy_Base_Class*> raw_enemy_list_for_spawner;

        // Eine Liste, die alle unsere Spawner-Objekte verwaltet.
        std::vector<std::unique_ptr<Enemy_Spawner>> spawner_list;

        // --- Zustandsvariablen für den Tod ---
        bool is_frozen_ = false;            // Friert das Spiel ein, wenn true
        float fade_to_black_alpha_ = 0.0f;  // Die aktuelle Transparenz für die Überblendung
        float fade_duration_ = 2.0f;        // Dauer der Überblendung in Sekunden

        bool puzzle_solved_ = false;
        std::vector<std::weak_ptr<Statue>> statues_in_level_;

        // ---  Für Fade-In Map ---
        float fade_alpha_ = 1.0f; // Start: schwarz
        float fadeIn_duration_ = 1.5f; // Sekunden
        bool fade_done_ = false;

    public:
        /**
         * @brief Konstruktor, wird einmal beim Erstellen der Szene aufgerufen.
         * Hier wird das Level initialisiert.
         */
        Level1Scene();

        /**
         * @brief Destruktor, wird beim Zerstören der Szene aufgerufen.
         */
        ~Level1Scene() override;

        /**
         * @brief Wird jeden Frame aufgerufen, um die Spiellogik zu aktualisieren.
         * Die Engine-Basisklasse `Scene` gibt den Namen "Update" vor.
         */
        void Update() override;

        /**
         * @brief Wird jeden Frame aufgerufen, um alles zu zeichnen.
         */
        void Draw() override;

        // --- HILFSFUNKTION ---
        void Add_Object_To_Waitlist(std::shared_ptr<Collidable> object);
    };
}