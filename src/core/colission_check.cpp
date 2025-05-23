//
// Created by Kruse on 23/05/2025.
//
#include "raylib.h"

bool Check_Collision(Rectangle rec1, Rectangle rec2)
{
    //allgemeine Kollisionserkennung von 2 Rechtecken

    bool collision = false;

    if (rec1.x < rec2.x + rec2.width &&
        rec1.x + rec1.width > rec2.x &&
        rec1.y < rec2.y + rec2.height &&
        rec1.y + rec1.height > rec2.y)
    {
        collision = true;
    }

    //gibt zurück, ob es eine Kollision gab
    return collision;
}


/*theoretisch gäbe es wohl noch einen viel einfacheren Weg den ich hier einfach mal
ausgeklammert reinnehme.

bool check_collision(Rectangle rec1, Rectangle rec2)
{
    bool collision = false;

    if (CheckCollisionRecs(rec1, rec2))
    {
        collision = true;
    }

    return collision;
}

Die Raylib verfügt wohl selbst mit "CheckCollisionRecs" über eine Funktion um die Kollision
zwischen Rechtecken zu prüfen. Keine Ahnung wie die läuft, aber falls was, kann man die oben
nutzen.*/