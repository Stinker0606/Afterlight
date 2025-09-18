#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include <raylib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <../../external/tileson/tileson.hpp>
#include "Cam.h"
#include "Object_Manager.h"
#include <string>
#include <map>
#include "CollisionManager.h"
#include "FogManager.h"

/**
 * @brief Eine erweiterte Version der Screen-Klasse der Engine.
 * Diese Klasse wird kopiert und erweitert, um benutzerdefinierte
 * Spielobjekte aus Tiled-Maps zu laden, ohne die originale Engine-Datei zu verändern.
 */
class LevelScreen
{
public:
    /**
     * @brief Konstruktor, der einen Pointer auf die aktuelle Level-Nummer entgegennimmt.
     * @param Level_Ptr Pointer auf die Integer-Variable, die die aktuelle Level-Nummer speichert.
     */
    LevelScreen(int* Level_Ptr);

    /**
     * @brief Destruktor, der die geladene Textur des Tilesets entlädt.
     */
    ~LevelScreen();

    /**
     * @brief Zeichnet die Tile-Layer der Map.
     * @param kamera Ein Shared Pointer auf das Kamera-Objekt.
     * @param aboveObjects Ein Boolean, der steuert, ob die Layer über (`true`) oder unter (`false`) den Spielobjekten gezeichnet werden.
     */
    void Draw_Level(std::shared_ptr<Cam> kamera, bool aboveObjects);

    /**
     * @brief Lädt alle Spielobjekte aus den Objekt-Layern der Tiled-Map.
     * Diese Funktion wird von uns erweitert, um unsere speziellen Objekte zu erkennen.
     * @param g_objectManager Eine Referenz zum zentralen Object_Manager, dem die neuen Objekte hinzugefügt werden.
     */
    void LoadGameObjects(Object_Manager& g_objectManager);

    /**
     * @brief Lädt die .json-Datei der Map und das dazugehörige Tileset.
     */
    void Load_Levelmap();
    void Hide_Tiles_In_Area(Rectangle area_to_hide);

    // Lädt eine spezifische Map anhand ihres Dateinamens.
    void LoadSpecificLevelmap(const std::string& map_filename);

    // Ein öffentliches "Gedächtnis" für alle Spawn-Punkte der Map.
    // Der Key ist der Name (z.B. "player_start_from_Test1"), der Value ist die Position.
    std::map<std::string, Vector2> spawn_points_;

    // Eine öffentliche Getter-Funktion, die die private Textur zurückgibt.
    Texture2D GetTileAtlasTexture() const;

    Vector2 GetMapSize() const;

private:
    Texture2D tileatlas_Texture;
    std::unique_ptr<tson::Map> map;
    int* Level_Nbr_Ptr = nullptr;
    bool loaded;
    std::vector<std::tuple<int, int>> hidden_tiles_;
};

#endif //LEVEL_SCREEN_H