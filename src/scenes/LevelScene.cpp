#include "LevelScene.h"
#include <memory>
#include "Store.h"
#include "PauseScene.h"
#include <string>
#include <vector>
#include "../config.h.in"
#include "../config_enemies.h.in"
#include "../game/Walls.h"
#include "../game/enemys/enemies_list.h"
#include "../game/interactables/interact_list.h"
#include "SoundManager.h"
#include "MenuScene.h"
#include "DeathScene.h"

using namespace std::string_literals;

namespace game::scenes
{
    static bool assets_preloaded = false;

    Level1Scene::Level1Scene() : pause_menu_()
    {
        // Verbirgt den Cursor wieder
        HideCursor();

        if (!assets_preloaded)
        {
            PreloadEnemyAssets();
            assets_preloaded = true;
        }

        // Starte die Ingame-Musik
        SoundManager::GetInstance().PlayMusic("ingame_music");

        levelScreen.LoadSpecificLevelmap(game::core::Store::next_scene_map);
        levelScreen.LoadGameObjects(objectManager);

        // 1. Hole die korrekten Map-Dimensionen aus dem geladenen Level
        Vector2 map_size = levelScreen.GetMapSize();

        // 2. Erstelle die world_bounds mit den echten Dimensionen der Map
        Rectangle world_bounds = {0, 0, map_size.x, map_size.y};

        // 3. Initialisiere den Collision Manager mit den korrekten Bounds
        p_cm = std::make_unique<Collision_Manager>(world_bounds, objectManager.managed_objects);


        // --- FINDE DEN KORREKTEN SPIELER-STARTPUNKT ---
        Vector2 player_start_pos = { 250, 250 }; // Fallback
        std::string spawn_name_to_find = game::core::Store::next_spawn_point;
        if (levelScreen.spawn_points_.count(spawn_name_to_find)) {
            player_start_pos = levelScreen.spawn_points_[spawn_name_to_find];
        } else {
            std::cout << "WARNUNG: Spawn-Punkt '" << spawn_name_to_find << "' nicht gefunden!" << std::endl;
        }

        // --- HOL DEN GLOBALEN SPIELER UND SETZE SEINE POSITION ---
        if (game::core::Store::player) {
            // 1. Hole den Pointer aus dem Store
            this->sp_player = game::core::Store::player;

            // 2. Füge den Spieler zu DIESER Szene hinzu
            objectManager.AddObject(game::core::Store::player);

            // 3. Setze seine Position auf den neuen Startpunkt
            game::core::Store::player->Set_Position(player_start_pos);

            // 4. Gib dem globalen Spieler die Referenz zum ObjectManager DIESER Szene.
            game::core::Store::player->Set_Object_Manager(&objectManager);
        }

        // 5. Kamera erstellen und an den Spieler binden (benutze den Store-Pointer)
        sp_cam = std::make_shared<Cam>(game::core::Store::player);

        // 6. Weise dem Spieler die Kamera zu.
        if (auto player = sp_player.lock()) {
            player->Set_Camera(sp_cam);
        }

        // 7. UI Manager anpassen
        uiManager_.SetPlayer(game::core::Store::player);

        // 8. Collision Manager initialisieren
        p_cm = std::make_unique<Collision_Manager>(world_bounds, objectManager.managed_objects);

        // --- NEBEL-INITIALISIERUNG ---

        // 9.1 Initialisiere die fogMaskTexture
        this->fogMaskTexture = LoadRenderTexture(game::Config::kStageWidth, game::Config::kStageHeight);

        // 9.2 Hole den Dateinamen der AKTUELLEN Map aus dem globalen Store.
        std::string map_filename = game::core::Store::next_scene_map;

        // 9.3 Initialisiere den FogManager mit dem korrekten, dynamischen Map-Namen.
        fogManager.InitializeFog(map_filename, {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});
        // -----------------------------------------

        // 10. Zeitmessung starten
        dtm.Start();

        // Finde alle Statuen im Level und speichere sie in unserer Liste
        for (const auto& obj : objectManager.managed_objects) {
            if (auto statue = std::dynamic_pointer_cast<Statue>(obj)) {
                statues_in_level_.push_back(statue);
            }
        }
    }

    Level1Scene::~Level1Scene()
    {
        // Gib den Speicher der RenderTexture frei wenn die Szene zerstört wird.
        UnloadRenderTexture(this->fogMaskTexture);
    }

    void game::scenes::Level1Scene::PreloadEnemyAssets()
    {
        std::cout << "Pre-spawning all enemy types to warm up the engine..." << std::endl;
        Vector2 offscreen_pos = {-10000.0f, -10000.0f};

        auto insect = std::make_shared<enemy::Insect_Monster>(offscreen_pos, objectManager, false);
        auto sniper = std::make_shared<enemy::DrownedSniper>(offscreen_pos, objectManager, false);
        auto corpse = std::make_shared<enemy::WalkingCorpse>(offscreen_pos, objectManager, false);
        auto wood_sniper = std::make_shared<enemy::WoodSniper>(offscreen_pos, objectManager, false);
        auto corpse2 = std::make_shared<enemy::Corpse>(offscreen_pos, objectManager, false);
        auto mimic = std::make_shared<enemy::Mimic>(offscreen_pos, objectManager, false);
        auto darkness_monster = std::make_shared<enemy::DarknessMonster>(offscreen_pos, objectManager, false);

        objectManager.AddObject(insect);
        objectManager.AddObject(sniper);
        objectManager.AddObject(corpse);
        objectManager.AddObject(wood_sniper);
        objectManager.AddObject(corpse2);
        objectManager.AddObject(mimic);
        objectManager.AddObject(darkness_monster);

        preload_dummies_.push_back(insect);
        preload_dummies_.push_back(sniper);
        preload_dummies_.push_back(corpse);
        preload_dummies_.push_back(wood_sniper);
        preload_dummies_.push_back(corpse2);
        preload_dummies_.push_back(mimic);
        preload_dummies_.push_back(darkness_monster);

        std::cout << "Engine warm-up complete. All subsequent spawns will be lag-free." << std::endl;
    }

    void Level1Scene::Add_Object_To_Waitlist(std::shared_ptr<Collidable> object)
    {
        if (object) {
            objects_to_add_list_.push_back(object);
        }
    }

    void Level1Scene::Update()
    {
        // --- PAUSEN-LOGIK ---
        if (IsKeyPressed(KEY_ESCAPE))
        {
            is_paused_ = !is_paused_;
            if (is_paused_) {
                ShowCursor();
                dtm.Pause(); // PAUSIERE DIE ZEIT
            } else {
                HideCursor();
                dtm.Resume(); // SETZE DIE ZEIT FORT
            }
        }

        if (is_paused_)
        {
            pause_menu_.Update();
            if (pause_menu_.IsResumeSelected())
            {
                is_paused_ = false;
                HideCursor();
                dtm.Resume(); // SETZE DIE ZEIT FORT
            }
            else if (pause_menu_.IsMainMenuSelected())
            {
                game::core::Store::player = std::make_shared<PlayerClass>(Vector2{0,0}, nullptr);
                game::core::Store::next_scene_map = "Tuto_0.json";
                game::core::Store::next_spawn_point = "player_start";
                game::core::Store::stage->ReplaceWithNewScene("gameplay"s, "menu"s, std::make_unique<MenuScene>());
            }
            return; // Stoppe alle weiteren Updates, wenn pausiert
        }

        dtm.Update();

        // --- GESAMTE SPIEL-LOGIK (wird nur ausgeführt, wenn nicht pausiert) ---
        if (!preload_dummies_.empty())
        {
            preload_death_timer_ -= GetFrameTime();
            if (preload_death_timer_ <= 0.0f)
            {
                for (auto& weak_dummy : preload_dummies_)
                {
                    if (auto dummy = weak_dummy.lock())
                    {
                        dummy->Mark_For_Destruction();
                    }
                }
                preload_dummies_.clear();
            }
        }

        // --- FADE-IN LOGIK Menu -> Scene 1 ---
        if (!fade_done_)
        {
            fade_alpha_ -= GetFrameTime() / fade_duration_;
            if (fade_alpha_ <= 0.0f)
            {
                fade_alpha_ = 0.0f;
                fade_done_ = true;
            }
        }

        if (auto player = sp_player.lock())
        {
            bool player_is_in_any_trigger = false;

        // --- DIALOG-LOGIK ---
        // 1. Prüfe zuerst, ob der Spieler in irgendeinem Trigger-Bereich ist.
        for (const auto& obj : objectManager.managed_objects) {
            if (auto trigger = std::dynamic_pointer_cast<DialogTrigger>(obj)) {
                if (CheckCollisionRecs(player->Get_Hitbox(), trigger->Get_Hitbox())) {
                    player_is_in_any_trigger = true;
                    // Wenn der Spieler "E" drückt UND der Dialog noch nicht aktiv ist, zeige ihn an.
                    if (IsKeyPressed(game::Config::key_Interact) && !dialogManager_.IsActive()) {
                        dialogManager_.ShowDialog(trigger->GetText(), trigger->GetName(), trigger->GetPortraitPath());
                    }
                    break; // Es kann immer nur ein Dialog aktiv sein.
                }
            }
        }

        // 2. Wenn ein Dialog aktiv ist...
        if (dialogManager_.IsActive()) {
            // ... und der Spieler keinen Trigger mehr berührt, schließe ihn.
            if (!player_is_in_any_trigger) {
                dialogManager_.HideDialog();
            }
            // ... oder wenn der Spieler erneut "E" drückt, schließe ihn ebenfalls.
            else if (IsKeyPressed(KEY_SPACE)) {
                dialogManager_.HideDialog();
            }
        }
            // 1. Prüfe, ob der Spieler gestorben ist und das Spiel noch nicht eingefroren ist.
            if (player->Is_Marked_For_Destruction() && !is_frozen_)
            {
                is_frozen_ = true; // Friere das Spiel ein
                SoundManager::GetInstance().StopCurrentMusic();
            }

            // 2. Wenn das Spiel eingefroren ist, kümmere dich um die Überblendung.
            if (is_frozen_)
            {
                // Erhöhe die Transparenz über Zeit
                fade_to_black_alpha_ += GetFrameTime() / fade_duration_;
                if (fade_to_black_alpha_ >= 1.4f)
                {
                    // Wenn die Überblendung komplett ist, wechsle zur DeathScene.
                    game::core::Store::stage->ReplaceWithNewScene("gameplay"s, "death"s, std::make_unique<DeathScene>(player->Get_Score()));
                    return;
                }
            }
            // 3. Die gesamte restliche Spiellogik wird nur ausgeführt, wenn das Spiel NICHT eingefroren ist.
            else
            {
                Vector2 player_position = player->Get_Player_Center();
                Vector2 player_center = player->Get_Player_Center();

                // --- INTELLIGENTE UPDATE-SCHLEIFE ---
                for (const auto& obj : objectManager.managed_objects)
                {
                    if (!obj) continue;

                    // 1. Versuche, das Objekt in einen Gegner umzuwandeln
                    if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(obj))
                    {
                        // Prüfe die Distanz zum Spieler, um die Animation zu aktivieren/deaktivieren
                        Vector2 obj_center = { obj->Get_Hitbox().x + obj->Get_Hitbox().width / 2, obj->Get_Hitbox().y + obj->Get_Hitbox().height / 2 };
                        float distance = Vector2Distance(player_center, obj_center);

                        // Wenn der Gegner im sichtbaren Radius ist, schalte die Animation an.
                        if (distance <= 99999999) {
                            enemy->Set_Animation_Active(true);
                        }
                        // Sonst schalte sie aus.
                        else {
                            enemy->Set_Animation_Active(false);
                        }
                        // 2. Rufe die spezifische KI jedes Gegners auf, ohne seinen Typ zu kennen!
                        enemy->Update_AI(dtm.Get_Dt(), player_position);
                    }
                    else
                    {
                        // 3. WENN es KEIN Gegner ist, rufe die normale Tick-Methode auf.
                        obj->Tick(dtm.Get_Dt());
                    }

                    // Prüfe auf Interaktion mit Dialog-Triggern
                    if (auto trigger = std::dynamic_pointer_cast<DialogTrigger>(obj)) {
                        if (CheckCollisionRecs(player->Get_Hitbox(), trigger->Get_Hitbox())) {
                            if (IsKeyPressed(game::Config::key_Interact)) {
                                dialogManager_.ShowDialog(trigger->GetText(), trigger->GetName(), trigger->GetPortraitPath());
                            }
                        }
                    }

                    // Logik zur Überprüfung des Rätsels
                    if (!puzzle_solved_ && !statues_in_level_.empty()) {
                        bool all_statues_solved = true;
                        for (const auto& weak_statue : statues_in_level_) {
                            if (auto statue = weak_statue.lock()) {
                                if (!statue->IsSolved()) {
                                    all_statues_solved = false;
                                    break;
                                }
                            }
                        }

                        if (all_statues_solved) {
                            puzzle_solved_ = true;
                            Vector2 key_pos = levelScreen.spawn_points_["key_spawn_point"];
                            Rectangle key_source_rect = { 256.0f, 160.0f, 32.0f, 32.0f };

                            auto key_item = std::make_shared<KeyConsumable>(key_pos, 1, levelScreen.GetTileAtlasTexture(), key_source_rect);

                            Add_Object_To_Waitlist(key_item);
                            SoundManager::GetInstance().PlaySfx("game_start");
                        }
                    }
                }

                if (player->Should_Place_Bomb())
                {
                    // Platziere die Bombe auf dem Grid, auf dem der Spieler steht
                    Vector2 bomb_pos = {
                        floorf(player_center.x / 32.0f) * 32.0f,
                        floorf(player_center.y / 32.0f) * 32.0f
                    };
                    auto bomb = std::make_shared<Bomb>(bomb_pos, this, objectManager);
                    Add_Object_To_Waitlist(bomb);
                }

                // --- "useFog"-Logik ---
                if (fogManager.IsFogActive())
                {
                    // WENN der Nebel AN ist, berechne die Transparenz basierend auf der Distanz.
                    Vector2 player_center = player->Get_Player_Center();
                    for (const auto& obj : objectManager.managed_objects) {
                        if (obj && obj->Get_Use_Fog()) {
                            Vector2 obj_center = { obj->Get_Hitbox().x + obj->Get_Hitbox().width / 2, obj->Get_Hitbox().y + obj->Get_Hitbox().height / 2 };
                            float distance = Vector2Distance(player_center, obj_center);
                            float alpha = 1.0f;
                            if (distance > game::Config::kFogFullVisibilityRadius) {
                                alpha = 1.0f - (distance - game::Config::kFogFullVisibilityRadius) / (game::Config::kFogNoVisibilityRadius - game::Config::kFogFullVisibilityRadius);
                            }
                            obj->Set_Visibility_Alpha(Clamp(alpha, 0.0f, 1.0f));
                        } else if (obj) {
                            // Objekte ohne useFog sind im Nebel immer voll sichtbar.
                            obj->Set_Visibility_Alpha(1.0f);
                        }
                    }
                }
                else
                {
                    // WENN der Nebel AUS ist, setze ALLE Objekte auf 100% Sichtbarkeit.
                    for (const auto& obj : objectManager.managed_objects) {
                        if (obj) {
                            obj->Set_Visibility_Alpha(1.0f);
                        }
                    }
                }

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
                Vector2 player_world_pos = player->Get_Player_Center();
                Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, sp_cam->cam);

                // Übergib die korrekten Bildschirm-Koordinaten an den FogManager.
                if (fogManager.IsFogActive()) {
                    fogManager.Update(player_screen_pos, dtm.Get_Dt());
                }

                // Aufräumen und Zeit aktualisieren
                dtm.Update();
            }
        }
    }

    void Level1Scene::Draw()
    {
        BeginDrawing();
        ClearBackground(Color{ 0, 32, 36, 255 }); // Hintergrundfarbe

        BeginMode2D(sp_cam->cam);
        {
            // Zeichne ZUERST den Boden (alle Ebenen UNTER den Objekten)
            // BEVOR der Nebel-Shader überhaupt aktiv wird.
            levelScreen.Draw_Level(sp_cam, false);

            // Starte den Nebel-Shader genau wie in deinem alten Code.
            // Er wird den Sichtkreis um den Spieler selbst zeichnen.
            fogManager.BeginFogMode();
            {
                // 1. ZEICHNE DEN BODEN ERNEUT
                // Dies ist notwendig, damit Objekte korrekt hinter Wänden verschwinden.
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

        // --- Dialogbox ---
        dialogManager_.Draw();

        // --- UI ---
        uiManager_.DrawUI(sp_cam->cam);

        if (is_frozen_)
        {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fade_to_black_alpha_));
        }

        // --- FADE-IN Menu -> Scene Overlay ---
        if (!fade_done_)
        {
            DrawRectangle(
                0, 0,
                game::Config::kStageWidth,
                game::Config::kStageHeight,
                Color{0, 0, 0, (unsigned char)(fade_alpha_ * 255)}
            );
        }

        // --- PAUSEN-OVERLAY ---
        if (is_paused_)
        {
            pause_menu_.Draw();
        }
    }
}