#include "Level1Scene.h"
#include <memory>
#include "Store.h"
#include "PauseScene.h"
#include <string>
#include <vector>
#include "../config.h.in"
#include "../game/Walls.h"
#include "../game/spawner/Level1Spawner.h"
#include "../game/enemys/enemies_list.h"


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
            max_enemies,
            objectManager
        ));

        // --- NEBEL-INITIALISIERUNG ---

        // 6.1 Initialisiere die fogMaskTexture
        this->fogMaskTexture = LoadRenderTexture(game::Config::kStageWidth, game::Config::kStageHeight);

        // 6.2 Hole den vollständigen Pfad der aktuellen Level-Map aus der Config.
        std::string map_path = game::Config::GetLevelMapPath(this->level_Nbr);

        // 6.3 Extrahiere nur den Dateinamen aus dem Pfad (z.B. "Swamp_0.json").
        // Dein FogManager erwartet nur den Namen, nicht den ganzen Pfad.
        std::string map_filename = map_path.substr(map_path.find_last_of("/\\") + 1);

        // 6.4 Initialisiere den FogManager mit dem dynamischen Map-Namen.
        fogManager.InitializeFog(map_filename, {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});
        // -----------------------------------------

        // 7. Zeitmessung starten
        dtm.Start();
    }

    Level1Scene::~Level1Scene()
    {
        // Gib den Speicher der RenderTexture frei, wenn die Szene zerstört wird.
        UnloadRenderTexture(this->fogMaskTexture);
    }

    void Level1Scene::Update()
    {
        // Standard-Engine-Input für die Pause-Funktion
        if (IsKeyPressed(KEY_ESCAPE))
        {
            game::core::Store::stage->SwitchToNewScene("pause"s, std::make_unique<PauseScene>());
        }
        if (IsKeyPressed(KEY_L)){
            ToggleFullscreen();
        }

        // Hole die Spielerposition EINMAL am Anfang des Frames.
        Vector2 player_position = sp_player->Get_Player_Center();

        // --- INTELLIGENTE UPDATE-SCHLEIFE ---
        for (const auto& obj : objectManager.managed_objects)
        {
            if (!obj) continue;

            // 1. Versuche, das Objekt in einen Gegner umzuwandeln
            if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(obj))
            {
                // 2. Rufe die spezifische KI jedes Gegners auf, ohne seinen Typ zu kennen!
                enemy->Update_AI(dtm.Get_Dt(), player_position);
            }
            else
            {
                // 3. WENN es KEIN Gegner ist, rufe die normale Tick-Methode auf.
                obj->Tick(dtm.Get_Dt());
            }
        }

        // --- "useFog"-Logik WIEDERHERSTELLEN ---
        // Dieser Block berechnet, wie transparent jedes Objekt sein soll.
        Vector2 player_center = sp_player->Get_Player_Center();
        for (const auto& obj : objectManager.managed_objects) {
            if (obj && obj->Get_Use_Fog()) {
                Vector2 obj_center = { obj->Get_Hitbox().x + obj->Get_Hitbox().width / 2, obj->Get_Hitbox().y + obj->Get_Hitbox().height / 2 };
                float distance = Vector2Distance(player_center, obj_center);
                float alpha = 1.0f;
                if (distance > game::Config::kFogFullVisibilityRadius) {
                    alpha = 1.0f - (distance - game::Config::kFogFullVisibilityRadius) / (game::Config::kFogNoVisibilityRadius - game::Config::kFogFullVisibilityRadius);
                }
                // Wir setzen die berechnete Transparenz für das Objekt.
                obj->Set_Visibility_Alpha(Clamp(alpha, 0.0f, 1.0f));
            } else if (obj) {
                // Alle anderen Objekte sind voll sichtbar.
                obj->Set_Visibility_Alpha(1.0f);
            }
        }
        // ------------------------------------

        // Aktualisiere Kamera und Kollisionen
        p_cm->Check_Collisions();
        sp_cam->Cam_Movement(dtm.Get_Dt());

        // --- KAMERA-BEGRENZUNG ---
        {
            // 1. Hole die halbe Bildschirmgröße. Die Kamera schaut von der Mitte aus.
            float screen_half_width = game::Config::kStageWidth / 2.0f;
            float screen_half_height = game::Config::kStageHeight / 2.0f;

            // 2. Berücksichtige den Zoom-Faktor. Bei Zoom 2.0 ist das Sichtfeld halb so groß.
            float zoomed_half_width = screen_half_width / sp_cam->cam.zoom;
            float zoomed_half_height = screen_half_height / sp_cam->cam.zoom;

            // 3. Berechne die minimal und maximal erlaubten Koordinaten für das KAMERA-ZIEL.
            float min_cam_x = game::Config::kWorldBoundsMinX + zoomed_half_width;
            float max_cam_x = game::Config::kWorldBoundsMaxX - zoomed_half_width;
            float min_cam_y = game::Config::kWorldBoundsMinY + zoomed_half_height;
            float max_cam_y = game::Config::kWorldBoundsMaxY - zoomed_half_height;

            // 4. "Klemme" die aktuelle Zielposition der Kamera an diese Grenzen.
            sp_cam->cam.target.x = Clamp(sp_cam->cam.target.x, min_cam_x, max_cam_x);
            sp_cam->cam.target.y = Clamp(sp_cam->cam.target.y, min_cam_y, max_cam_y);
        }

        // --- NEBEL-UPDATE (aus deinem funktionierenden Code übernommen) ---
        // Wir müssen die WELT-Position des Spielers in BILDSCHIRM-Koordinaten umrechnen.
        Vector2 player_world_pos = sp_player->Get_Player_Center();
        Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, sp_cam->cam);

        // Übergib die korrekten Bildschirm-Koordinaten an den FogManager.
        if (fogManager.IsFogActive()) {
            fogManager.Update(player_screen_pos, dtm.Get_Dt());
        }

        // Aufräumen und Zeit aktualisieren
        objectManager.Cleanup_Objects();
        dtm.Update();
    }

    void Level1Scene::Draw()
    {
        BeginDrawing();
        ClearBackground((Color){ 0, 32, 36, 255}); // Deine Hintergrundfarbe

        BeginMode2D(sp_cam->cam);
        {
            // --- TEIL 1: OBJEKTE MIT NEBEL-SHADER ---
            // Wir starten den Shader...
            fogManager.BeginFogMode();
            {
                // ...zeichnen die Tile-Layer (die immer betroffen sind)...
                levelScreen.Draw_Level(sp_cam, false);

                // ...und zeichnen NUR die Objekte, die KEIN useFog haben.
                for (const auto& obj : objectManager.managed_objects) {
                    if (obj && !obj->Get_Use_Fog()) { // Beachte das "!"
                        obj->Draw();
                    }
                }
                levelScreen.Draw_Level(sp_cam, true);
            }
            // ...und beenden den Shader wieder.
            fogManager.EndFogMode();


            // --- TEIL 2: OBJEKTE MIT TRANSPARENZ (OHNE SHADER) ---
            // Der Shader ist jetzt aus. Wir zeichnen jetzt alle Objekte, die useFog haben.
            // Ihre Transparenz wird durch den `visibility_alpha`-Wert gesteuert,
            // den wir in der Update()-Methode berechnen.
            for (const auto& obj : objectManager.managed_objects) {
                if (obj && obj->Get_Use_Fog()) {
                    obj->Draw();
                }
            }

            // --- DEBUG: Hitboxen ---
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
        EndMode2D();
    }
}