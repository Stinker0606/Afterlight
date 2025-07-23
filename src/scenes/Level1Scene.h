#pragma once

#include "Scene.h"
#include "LevelScreen.h"
#include "Object_Manager.h"
#include "CollisionManager.h"
#include "../core/DeltaTimeMachine.h"
#include "Cam.h"
#include "../game/PlayerClassOne.h"
#include "FogManager.h"

#include <memory>
#include <vector>

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
        // --- Engine-Systeme ---
        Object_Manager objectManager;
        DT::timemachine dtm;
        std::unique_ptr<Collision_Manager> p_cm;

        // --- Spiel-Objekte ---
        std::shared_ptr<Player_Class_One> sp_player;
        std::shared_ptr<Cam> sp_cam;

        // --- Deine eigenen Systeme für diesen Level ---
        FogManager fogManager;

        // --- Level-spezifische Daten ---
        int level_Nbr = 1;
        LevelScreen levelScreen{&level_Nbr}; // Wir benutzen unsere erweiterbare LevelScreen

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
    };
}