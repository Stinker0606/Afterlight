// Note: Probably you will never want to make changes directly to this file or
// in the main function! Add your code to the existing scene files under
// src/scenes and/or create new scenes.
// General settings can be made in the config.h.in file.

#include <string>
#include <raylib.h>
#include <game.h>
#include "../scenes/scenes.h"
#include "scenes/screen.h"
#include "config.h.in"
#include "Walls.h"
#include "consumables.h"
#include "enemy_base_class.h"

using namespace std::string_literals;

//RenderTexture2D canvas= LoadRenderTexture(940,560);



int main() {
    game::core::Game game(game::Config::kStageWidth, game::Config::kStageHeight, game::Config::kFullScreen,
                          game::Config::kTargetFps, game::Config::kWindowFlags, game::Config::kTextureFilter,
                          game::Config::kExitKey,game::Config::kUseMouse, game::Config::kAudio, game::Config::kProjectName);

    //intizialize screen object
    //Screen screen;

    game.Run("menu"s, std::make_unique<game::scenes::MenuScene>());

    //call draw_Level function from screen
    // screen.draw_Level();

    return EXIT_SUCCESS;
}