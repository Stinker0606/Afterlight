//
// Created by $Will on 01.06.2025.
//

#ifndef WALL_COLLISIONS_H
#define WALL_COLLISIONS_H

#include "raylib.h"
#include <string>
#include "DeltaTimeMachine.h"
#include "Playerinput.h"


namespace game::core
{

    // Basisklasse für Wände
    class Wall
    {
    protected:
        // Wände verfügen über ein Rectangle also über eine Größe und Position
        Rectangle rect;
        // Wände verfügen über einen type
        std::string type;

    public:
        // Konstruktor initialisiert die Wand mit Rectangle und type
        Wall(Rectangle rectangle, const std::string& wallType)
            : rect(rectangle), type(wallType) {}

        // Destruktor
        virtual ~Wall() = default;

        // Wird von Subklassen überschrieben, um Wandverhalten zu ändern
        virtual void Update(float dt) {}

        /* Überprüft, ob eine Kollision zwischen einer Wand und dem
        Spielerrec aus Playerinput.cpp erfolgt */
        virtual bool CheckCollision(const Rectangle& playerRect) const {
            return CheckCollisionRecs(rect, playerRect);
        }

        // Getter und Setter
        Rectangle GetRect() const { return rect; }
        void SetRect(Rectangle r) { rect = r; }

        std::string GetType() const { return type; }
    };

    // Movable Wall erbt von der standard Wall
    class Movable_Wall : public Wall    //DIESE KLASSE IST TEAM INTERN UND SOLL NICHT INS BASISSYSTEM
    {
    private:
        Vector2 direction;      // Bewegungsrichtung
        float speed;            // Bewegungsgeschwindigkeit
        bool isMoving;          // Bewegt sich die Wand gerade?
        float movedDistance;    // Wie weit wurde sie schon verschoben

    public:
        Movable_Wall(Rectangle rectangle, const std::string& wallType, Vector2 moveDir, float moveSpeed)
            : Wall(rectangle, wallType), direction(moveDir), speed(moveSpeed),
              isMoving(false), movedDistance(0.0f) {}

        // Löst die Bewegung aus
        void TriggerMove(Player::Direction direction);

        // Hier findet die Bewegung statt
        void Update(float dt) override;

        // Getter für die Bewegungsrichtung
        Vector2 GetDirection() const { return direction; }

    };

    void HandleWallCollisions(Rectangle& playerRect, float& playerX,
    float& playerY, float playerSpeed, DT::timemachine& deltaTimeMachine,
    Player::Direction dir);

}
#endif //WALL_COLLISIONS_H
