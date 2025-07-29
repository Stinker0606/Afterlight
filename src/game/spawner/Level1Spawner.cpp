#include "Level1Spawner.h"
#include "../config.h.in"

// Enemy List
#include "../enemys/InsectMonster.h"
// #include "../enemys/PLACEHOLDER.h" //PLACEHOLDER


// Der Konstruktor ruft einfach den Konstruktor der Basisklasse auf.
Level1_Spawner::Level1_Spawner(Rectangle spawner_Area,
                               const std::vector<Rectangle>& obstacle_List,
                               std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                               float spawn_Rate,
                               int max_Enemies)
    : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List, spawn_Rate, max_Enemies)
{
    // KORREKTUR: Wir setzen die Hitbox der Collidable-Basisklasse explizit,
    // damit der Spawner an der richtigen Position in der Spielwelt existiert.
    this->hitbox = spawner_Area;

    // Lade das Sprite für diesen Spawner-Typ aus der Config.
    this->spawner_sprite = LoadTexture(game::Config::kSpawnerLevel1SpritePath);
}

// Der Destruktor, um Speicherlecks zu vermeiden
Level1_Spawner::~Level1_Spawner()
{
    // Stellt sicher, dass die Textur entladen wird, um Speicherlecks zu vermeiden.
    if (spawner_sprite.id > 0)
    {
        UnloadTexture(this->spawner_sprite);
    }
}

// Wir überschreiben die `Collidable::Tick`-Methode...
void Level1_Spawner::Tick(float delta_time)
{
    // ...und rufen von hier aus manuell die `EnemySpawner::Tick`-Methode auf,
    // in der die eigentliche Logik zum Erstellen von Gegnern steht.
    Enemy_Spawner::Tick(delta_time);
}

// Die Draw-Funktion zeichnet das Sprite an der korrekten Hitbox-Position.
void Level1_Spawner::Draw()
{
    // Wir benutzen die `visibility_alpha`-Eigenschaft, die jedes Collidable jetzt hat,
    // damit der Spawner später auch vom Nebel betroffen sein kann.
    DrawTextureV(this->spawner_sprite, {(float)this->hitbox.x, (float)this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

// Hier passiert die Magie: Der Spawner wählt einen zufälligen Gegner aus.
enemy::Enemy_Base_Class* Level1_Spawner::createEnemy(Vector2 position)
{
    // Diese Funktion bleibt unverändert.
    std::vector<std::string> enemy_pool = {"Insect_Monster"};
    int random_index = GetRandomValue(0, (int)enemy_pool.size() - 1);
    std::string enemy_to_spawn = enemy_pool[random_index];

    if (enemy_to_spawn == "Insect_Monster")
    {
        return new enemy::Insect_Monster(position);
    }

    return nullptr;
}