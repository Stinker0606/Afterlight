#pragma once

#include "../EnemySpawner.h"
#include <vector>
#include <string>

/**
 * @brief Der Standard-Spawner für Level 1 (Sumpf).
 * Diese Klasse ist dafür verantwortlich, zufällig verschiedene Gegner
 * aus einem vordefinierten "Sumpf-Pool" zu erzeugen.
 */
class Level1_Spawner : public Enemy_Spawner
{
public:
    /**
     * @brief Konstruktor für den Level-1-Spawner.
     * Nimmt exakt die 5 Argumente entgegen, die die Basisklasse Enemy_Spawner erwartet.
     * @param spawner_Area Die Position und Größe des Spawners aus Tiled.
     * @param obstacle_List Eine Referenz auf eine Liste mit Hindernis-Rechtecken.
     * @param enemy_List Eine Referenz auf eine Liste, in die die neuen Gegner-Pointer eingefügt werden.
     * @param spawn_rate Gegner pro Sekunde (kann aus Tiled kommen).
     * @param max_enemies Maximale Anzahl an Gegnern (kann aus Tiled kommen).
     */
    Level1_Spawner(Rectangle spawner_Area,
                   const std::vector<Rectangle>& obstacle_List,
                   std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                   float spawn_rate,
                   int max_enemies);

    ~Level1_Spawner() override;

private:
    Texture2D spawner_sprite;

    /**
     * @brief Zeichnet den Spawner. Ist leer, da Spawner unsichtbar sind.
     * Erfüllt den "Vertrag" der Collidable-Basisklasse.
     */
    void Draw() override;

protected:
    /**
     * @brief Wählt zufällig einen Gegner aus dem Sumpf-Pool aus und erstellt ihn.
     * @param position Die Position, an der der Gegner erscheinen soll.
     * @return Ein roher Pointer auf den neu erstellten Gegner, wie von der Basisklasse gefordert.
     */
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override;
};