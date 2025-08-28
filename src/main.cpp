// Note: Probably you will never want to make changes directly to this file or
// in the main function! Add your code to the existing scene files under
// src/scenes and/or create new scenes.
// General settings can be made in the config.h.in file.

#include <string>
#include <raylib.h>
#include <Game.h>
#include "scenes/Screen.h"
#include "config.h.in"
#include "scenes/MenuScene.h"
#include "scenes/LevelScene.h"
#include "core/Store.h"

using namespace std::string_literals;

int main()
{

    game::core::Game game(game::Config::kStageWidth, game::Config::kStageHeight, game::Config::kFullScreen,
                          game::Config::kTargetFps, game::Config::kWindowFlags, game::Config::kTextureFilter,
                          game::Config::kExitKey,game::Config::kUseMouse, game::Config::kAudio,
                          game::Config::kProjectName);

    HideCursor();

    // 1. Setze die Start-Map und den Spawnpoint für den Start.
    game::core::Store::next_scene_map = "Tuto_0.json";
    game::core::Store::next_spawn_point = "player_start";

    // 2. ERSTELLE DEN SPIELER EINMALIG.
    // Object_Manager ist hier nur temporär nötig, wird in der Szene richtig gesetzt.
    game::core::Store::player = std::make_shared<PlayerClass>(Vector2{0,0}, nullptr);

    // 3. Starte das Spiel mit der MenuScene.
    game.Run("menu"s, std::make_unique<game::scenes::MenuScene>());

    return EXIT_SUCCESS;
}