﻿// Note: Probably you will never want to make changes directly to this file or
// in the main function! Add your code to the existing scene files under
// src/scenes and/or create new scenes.
// General settings can be made in the config.h.in file.

#include <string>
#include <raylib.h>
#include <Game.h>
#include "scenes/Screen.h"
#include "config.h.in"
#include "MenuScene.h"


/* Aktiviert "String-Literalen erlaubt "menu"s Daraus wird dann direkt ein std::string
 * also "menu"s == std::string ("menu")
 */

using namespace std::string_literals;

int main()
{

    game::core::Game game(game::Config::kStageWidth, game::Config::kStageHeight, game::Config::kFullScreen,
                          game::Config::kTargetFps, game::Config::kWindowFlags, game::Config::kTextureFilter,
                          game::Config::kExitKey,game::Config::kUseMouse, game::Config::kAudio,
                          game::Config::kProjectName);

    game.Run("menu"s, std::make_unique<game::scenes::MenuScene>());

    return EXIT_SUCCESS;
}