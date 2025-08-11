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
#include "../game/interactables/interact_list.h"

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

        // 5. Weise dem Spieler die Kamera zu.
        sp_player->Set_Camera(sp_cam);

        // 6. Collision Manager initialisieren
        Rectangle world_bounds = {0, 0, 4000, 4000};
        p_cm = std::make_unique<Collision_Manager>(world_bounds, objectManager.managed_objects);

        // --- NEBEL-INITIALISIERUNG ---

        // 7.1 Initialisiere die fogMaskTexture
        this->fogMaskTexture = LoadRenderTexture(game::Config::kStageWidth, game::Config::kStageHeight);

        // 7.2 Hole den vollständigen Pfad der aktuellen Level-Map aus der Config.
        std::string map_path = game::Config::GetLevelMapPath(this->level_Nbr);

        // 7.3 Extrahiere nur den Dateinamen aus dem Pfad (z.B. "Swamp_0.json").
        // Dein FogManager erwartet nur den Namen nicht den ganzen Pfad.
        std::string map_filename = map_path.substr(map_path.find_last_of("/\\") + 1);

        // 7.4 Initialisiere den FogManager mit dem dynamischen Map-Namen.
        fogManager.InitializeFog(map_filename, {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});
        // -----------------------------------------

        // 8. Zeitmessung starten
        dtm.Start();
    }

    Level1Scene::~Level1Scene()
    {
        // Gib den Speicher der RenderTexture frei wenn die Szene zerstört wird.
        UnloadRenderTexture(this->fogMaskTexture);
    }

    void Level1Scene::Add_Object_To_Waitlist(std::shared_ptr<Collidable> object)
    {
        if (object) {
            objects_to_add_list_.push_back(object);
        }
    }

    void Level1Scene::Update()
    {
        // Standard-Engine-Inputs
        if (IsKeyPressed(KEY_ESCAPE))
        {
            game::core::Store::stage->SwitchToNewScene("pause"s, std::make_unique<PauseScene>());
        }
        if (IsKeyPressed(KEY_L)){
            ToggleFullscreen();
        }

        // Hole die Spielerposition einmalig am Anfang des Frames.
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

        if (sp_player->Should_Place_Bomb())
        {
            // Platziere die Bombe auf dem Grid, auf dem der Spieler steht
            Vector2 player_center = sp_player->Get_Player_Center();
            Vector2 bomb_pos = {
                floorf(player_center.x / 32.0f) * 32.0f,
                floorf(player_center.y / 32.0f) * 32.0f
            };
            auto bomb = std::make_shared<Bomb>(bomb_pos, this);
            Add_Object_To_Waitlist(bomb);
        }

        // --- "useFog"-Logik ---
        // Diese Logik berechnet wie transparent jedes Objekt sein soll.
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

        objectManager.Cleanup_Objects();

        for (const auto& new_obj : objects_to_add_list_) {
            objectManager.AddObject(new_obj);
        }
        objects_to_add_list_.clear();

        // Aktualisiere Kamera und Kollisionen
        p_cm->Check_Collisions();
        sp_cam->Cam_Movement(dtm.Get_Dt());

        // --- KAMERA-BEGRENZUNG ---
        {
            // 1. Hole die halbe Bildschirmgröße. Die Kamera schaut von der Mitte aus.
            float screen_half_width = game::Config::kStageWidth / 2.0f;
            float screen_half_height = game::Config::kStageHeight / 2.0f;

            // 2. Berücksichtige den Zoom-Faktor.
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

        // --- NEBEL-UPDATE ---
        // Wir müssen die WELT-Position des Spielers in BILDSCHIRM-Koordinaten umrechnen.
        Vector2 player_world_pos = sp_player->Get_Player_Center();
        Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, sp_cam->cam);

        // Übergib die korrekten Bildschirm-Koordinaten an den FogManager.
        if (fogManager.IsFogActive()) {
            fogManager.Update(player_screen_pos, dtm.Get_Dt());
        }

        // Aufräumen und Zeit aktualisieren
        dtm.Update();
    }

    void Level1Scene::Draw()
    {
        BeginDrawing();
        ClearBackground((Color){ 0, 32, 36, 255 }); // Hintergrundfarbe

        BeginMode2D(sp_cam->cam);
        {
            // Starte den Nebel-Shader genau wie in deinem alten Code.
            // Er wird den Sichtkreis um den Spieler selbst zeichnen.
            fogManager.BeginFogMode();
            {
                // 1. ZEICHNE DEN BODEN
                // (Alle Kachel-Ebenen, bei denen "IsAboveObjects" NICHT true ist)
                levelScreen.Draw_Level(sp_cam, false);

                // 2. SORTIERE ALLE SPIELOBJEKTE
                // Hier werden Spieler, Gegner, Bäume etc. (sobald sie Objekte sind)
                // und alle anderen Objekte in EINER Liste korrekt sortiert.
                std::sort(objectManager.managed_objects.begin(), objectManager.managed_objects.end(),
                    [](const std::shared_ptr<Collidable>& a, const std::shared_ptr<Collidable>& b) {
                        // Sortiere nach der Unterkante der Hitbox
                        return (a->Get_Hitbox().y + a->Get_Hitbox().height) < (b->Get_Hitbox().y + b->Get_Hitbox().height);
                    });

                // 3. ZEICHNE ALLE SORTIERTEN OBJEKTE
                // Diese EINE Schleife zeichnet jetzt alles in der richtigen Reihenfolge.
                // Jedes Objekt nutzt seine eigene `visibility_alpha` für den Entfernungs-Fade.
                for (const auto& obj : objectManager.managed_objects) {
                    if (obj) {
                        obj->Draw();
                    }
                }

                // 4. ZEICHNE DIE "IMMER-OBEN"-SCHICHT
                // (z.B. Wandspitzen, die immer über dem Spieler sein müssen)
                levelScreen.Draw_Level(sp_cam, true);
            }
            // Beende den Shader.
            fogManager.EndFogMode();

            // --- DEBUG ---
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