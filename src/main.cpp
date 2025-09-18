// Note: Probably you will never want to make changes directly to this file or
// in the main function! Add your code to the existing scene files under
// src/scenes and/or create new scenes.
// General settings can be made in the config.h.in file.

#include <string>
#include <raylib.h>
#include <Game.h>
#include "scenes/Screen.h"
#include "config.h.in"
#include "SplashScreenScene.h"
#include "scenes/MenuScene.h"
#include "scenes/LevelScene.h"
#include "core/Store.h"
#include "core/SoundManager.h"
#include "core/SettingsManager.h"

using namespace std::string_literals;

int main()
{
    // Lade die gespeicherten Einstellungen
    SettingsManager::GetInstance().LoadSettings();

    // Setze ALLE Konfigurations-Flags in EINEM Aufruf, BEVOR das Fenster erstellt wird.
    unsigned int flags = FLAG_WINDOW_HIDDEN; // Unsichtbar starten, um weißen Blitz zu vermeiden
    if (SettingsManager::GetInstance().IsVsyncEnabled()) {
        flags |= FLAG_VSYNC_HINT; // Füge V-Sync hinzu, falls in den Settings aktiviert
    }
    SetConfigFlags(flags);

    // Erstelle das Spiel-Objekt (dieser Aufruf enthält InitWindow)
    game::core::Game game(game::Config::kStageWidth, game::Config::kStageHeight, game::Config::kFullScreen,
                          game::Config::kTargetFps, game::Config::kWindowFlags, game::Config::kTextureFilter,
                          game::Config::kExitKey,game::Config::kUseMouse, game::Config::kAudio,
                          game::Config::kProjectName);

    // Wende den Fenstermodus an, NACHDEM das Fenster erstellt wurde
    SettingsManager::GetInstance().ApplyDisplaySettings();

    HideCursor();

    // SoundManager initialisieren
    SoundManager::GetInstance().Init();

    // 1. Setze die Start-Map und den Spawnpoint für den Start.
    game::core::Store::next_scene_map = "Tuto_0.json";
    game::core::Store::next_spawn_point = "player_start";

    // 2. ERSTELLE DEN SPIELER EINMALIG.
    game::core::Store::player = std::make_shared<PlayerClass>(Vector2{0,0}, nullptr);

    // 3. Starte das Spiel mit der Splash-Szene.
    game.Run("splash"s, std::make_unique<game::scenes::SplashScreenScene>());

    // Alle Sound-Ressourcen am Ende freigeben
    SoundManager::GetInstance().UnloadAll();

    return EXIT_SUCCESS;
}