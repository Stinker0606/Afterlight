#include "Level1Scene.h"
#include <memory>
#include "Store.h"
#include "PauseScene.h"
#include "../config.h.in"

using namespace std::string_literals;

namespace game::scenes
{
    Level1Scene::Level1Scene()
    {
        // 1. Lade die statischen Level-Objekte (Wände etc.) aus der Tiled-Map.
        // Wir rufen die LoadGameObjects-Methode unserer neuen LevelScreen auf.
        // Diese wird später unsere benutzerdefinierten Objekte laden.
        levelScreen.LoadGameObjects(objectManager);

        // 2. Spieler erstellen und zum Manager hinzufügen
        // HINWEIS: Die Startposition wird hier noch fest einprogrammiert.
        // Später holen wir sie aus dem "player_start"-Objekt in der Tiled-Map.
        Vector2 player_start_pos = {250, 250};
        sp_player = std::make_shared<Player_Class_One>(player_start_pos, objectManager);
        objectManager.AddObject(sp_player);

        // 3. Kamera erstellen und an den Spieler binden
        sp_cam = std::make_shared<Cam>(sp_player);

        // 4. Collision Manager initialisieren
        Rectangle world_bounds = {0, 0, 4000, 4000}; // Großer Bereich für die Kollisionserkennung
        p_cm = std::make_unique<Collision_Manager>(world_bounds, objectManager.managed_objects);

        // 5. Deinen FogManager für diesen Level initialisieren
        fogManager.InitializeFog("Swamp_1", {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});

        // 6. Zeitmessung der Engine starten
        dtm.Start();
    }

    Level1Scene::~Level1Scene()
    {
        // Hier kommt später Aufräumcode hin.
    }

    void Level1Scene::Update()
    {
        // Standard-Engine-Input für die Pause-Funktion
        if (IsKeyPressed(KEY_ESCAPE))
        {
            game::core::Store::stage->SwitchToNewScene("pause"s, std::make_unique<PauseScene>());
        }

        // Rufe die Tick-Methode für ALLE Objekte im Spiel auf (Spieler, Spawner, Gegner, Projektile...).
        for (const auto& obj : objectManager.managed_objects)
        {
            if (obj) obj->Tick(dtm.Get_Dt());
        }

        // Aktualisiere deine Level-spezifischen Systeme
        sp_cam->Cam_Movement(dtm.Get_Dt());
        if (fogManager.IsFogActive())
        {
           fogManager.Update(sp_player->Get_Player_Center(), dtm.Get_Dt());
        }

        // Prüfe alle Kollisionen für diesen Frame
        p_cm->Check_Collisions();

        // Entferne alle Objekte, die zur Zerstörung markiert wurden
        objectManager.Cleanup_Objects();

        // Aktualisiere den DeltaTime-Timer für den nächsten Frame
        dtm.Update();
    }

    void Level1Scene::Draw()
    {
        BeginDrawing();
        ClearBackground((Color){ 0, 32, 36, 255}); // Deine gewünschte Hintergrundfarbe

        // Starte den 2D-Kameramodus
        BeginMode2D(sp_cam->cam);

            // 1. Zeichne die Tile-Layer, die UNTER dem Spieler liegen
            levelScreen.Draw_Level(sp_cam, false);

            // 2. Zeichne alle Spiel-Objekte
            for (const auto& obj : objectManager.managed_objects)
            {
                if (obj) obj->Draw();
            }

            // 3. Zeichne die Tile-Layer, die ÜBER dem Spieler liegen
            levelScreen.Draw_Level(sp_cam, true);

        // Beende den 2D-Kameramodus
        EndMode2D();

        // 4. Zeichne den Nebel-Shader als letzten Schritt über das gesamte Bild
        if (fogManager.IsFogActive())
        {
            fogManager.BeginFogMode();
                // Wir zeichnen ein leeres Rechteck über den Bildschirm, damit der Shader-Effekt greift
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLANK);
            fogManager.EndFogMode();
        }

        EndDrawing();
    }
}
