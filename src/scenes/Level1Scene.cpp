#include "Level1Scene.h"
#include <memory>
#include "Store.h"
#include "PauseScene.h"
#include <string>
#include <vector>
#include "../config.h.in"
#include "../game/Walls.h"
#include "../game/spawner/Level1Spawner.h"

using namespace std::string_literals;

namespace game::scenes
{
    Level1Scene::Level1Scene()
    {
        // 1. Lade die statischen Level-Objekte (Wände etc.) aus der Tiled-Map.
        // Wir rufen die LoadGameObjects-Methode unserer neuen LevelScreen auf.
        // Diese wird später unsere benutzerdefinierten Objekte laden.
        levelScreen.LoadGameObjects(objectManager);

        // 2. Finde den Spieler-Startpunkt, den die LevelScreen geladen hat.
        Vector2 player_start_pos = {250, 250}; // Standard-Position, falls kein Startpunkt gefunden wird
        for (const auto& obj : objectManager.managed_objects) {
            // Wir müssen einen Weg finden, den Startpunkt zu identifizieren.
            // Vorerst bleibt es bei der festen Position.
        }

        // 3. Spieler erstellen und zum Manager hinzufügen
        sp_player = std::make_shared<PlayerClass>(player_start_pos, objectManager);
        objectManager.AddObject(sp_player);

        // 4. Kamera erstellen und an den Spieler binden
        sp_cam = std::make_shared<Cam>(sp_player);

        // 5. Collision Manager initialisieren
        Rectangle world_bounds = {0, 0, 4000, 4000};
        p_cm = std::make_unique<Collision_Manager>(world_bounds, objectManager.managed_objects);

        // --- Spawner Erstellung ---

        // Die Listen werden hier in der Szene erstellt.
        // `obstacle_list_for_spawner` ist noch leer, aber sie existiert.
        // `raw_enemy_list_for_spawner` ist die temporäre Liste für neue Gegner.

        Rectangle spawner_area = { 400, 400, 300, 200 };
        float spawn_rate = 0.5f;
        int max_enemies = 5;

        // Erstelle den Spawner.
        // `std::make_unique` ist nicht ideal, da die Basisklasse `Enemy_Spawner` nicht von `Collidable` erbt
        // und wir den Pointer in einem `unique_ptr<Enemy_Spawner>` speichern wollen.
        // Wir erstellen ihn daher direkt.

        spawner_list.push_back(std::make_unique<Level1_Spawner>(
            spawner_area,
            obstacle_list_for_spawner,
            raw_enemy_list_for_spawner,
            spawn_rate,
            max_enemies
        ));

        // --- NEBEL-INITIALISIERUNG ---

        // 1. Hole den vollständigen Pfad der aktuellen Level-Map aus der Config.
        std::string map_path = game::Config::GetLevelMapPath(this->level_Nbr);

        // 2. Extrahiere nur den Dateinamen aus dem Pfad (z.B. "Swamp_0.json").
        // Dein FogManager erwartet nur den Namen, nicht den ganzen Pfad.
        std::string map_filename = map_path.substr(map_path.find_last_of("/\\") + 1);

        // 3. Initialisiere den FogManager mit dem dynamischen Map-Namen.
        fogManager.InitializeFog(map_filename, {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});
        // -----------------------------------------

        // 6. Zeitmessung starten
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
        ClearBackground((Color){ 0, 32, 36, 255}); // Deine Hintergrundfarbe

        // Zeichne die unteren Tile-Layer
        levelScreen.Draw_Level(sp_cam, false);

        // Starte den 2D-Kameramodus
        BeginMode2D(sp_cam->cam);

        // Zeichne alle Spiel-Objekte
        for (const auto& obj : objectManager.managed_objects)
        {
            if (obj) obj->Draw();
        }

        // HINWEIS: Wir rufen EndMode2D() und EndDrawing() NICHT explizit auf,
        // da die Engine dies im Hintergrund zu tun scheint.
        // Stattdessen zeichnen wir die oberen Layer und den Nebel.

        // Zeichne die oberen Tile-Layer
        levelScreen.Draw_Level(sp_cam, true);

        // Zeichne den Nebel-Shader als letzten Schritt über das gesamte Bild
        if (fogManager.IsFogActive())
        {
            fogManager.BeginFogMode();
            // Wir zeichnen ein leeres Rechteck über den Bildschirm, damit der Shader-Effekt greift
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLANK);
            fogManager.EndFogMode();

        // Debug Hitboxen
        if (game::Config::kDebugShowHitboxes)
        {
            for (const auto& p_object : objectManager.managed_objects)
            {
                if (p_object != nullptr)
                {
                    DrawRectangleLinesEx(p_object->Get_Hitbox(), 2.0f, RED);
                }
            }
        }
        }
    }
}
