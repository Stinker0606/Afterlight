//
// Created by $Will on 11.06.2025.
//

#include "CollisionManager.h"

// Konstruktor
Collision_Manager::Collision_Manager(Rectangle world_Bounds)
    : quadtree(std::make_unique<Quadtree>(0, world_Bounds)){}

void Collision_Manager::Regist_Object(Collidable* object)
{
    // push_back nimmtdas Element "objecte" also einen Collidable zeiger (eine Adresse) und speichert diese
    // am Ende des collidables Vector. Vektor vergrößert bei Bedarf seinen Speicher automatisch.
    collidables.push_back(object);
}

void Collision_Manager::Unregist_Object(Collidable* object)
{
    // "begin" ist ein Iterator also ein Zeiger welcher auf das erste element im Vektor zeigt
    // "end" zeigt auf die Position nach dem letzten Element und sagt, dass it dort nicht sein darf
    // heißt er iteriert solange durch den Vektor bis er die passende Stelle gefunden hat, und
    // durch .erase löscht er dise heraus und alle anderen Obejcte (pointer) im Vektor rücken auf die leere Stelle nach
    for (auto it = collidables.begin(); it != collidables.end(); ++it)
    {
        if (*it == object)
        {
            collidables.erase(it);
            break;
        }
    }
}

void Collision_Manager::Check_Collisions()
{
    // Ruft die Clear des Quadtrees auf. Löscht nicht den Quadtree aber alle seine Unterknoten
    quadtree->Clear();

    // Range-based for loop. Für jedes Element aus dem Container "collidables" wird eine tomporäre Kopie namens obj erstellt
    // Jeder Zeiger ruft die Insert(obj) auf und befüllt den Quadtree für diesen Frame mit allen Objekten
    for (Collidable* obj : collidables)
    {
        quadtree->Insert(obj);
    }

    // set ist ein spezieller Container welcher jeden Wert nur genau einmal in sich tragen darf
    std::set<std::pair<Collidable*, Collidable*>> processed_pairs;

    // Doppeltschleide zur Kollisionsprüfung
    for (Collidable* objA : collidables)
    {
        // Vector welcher mit der Quadtree Retrieve potenziele Objekte in die Liste ladet, mit welcher unser ObjA
        // Kollidieren könnte, also Objekte im eigenen Knoten oder einen Drüber weil diese auf der LInie liegen könnten
        std::vector<Collidable*> candidates = quadtree->Retrieve(objA);

        for (Collidable* objB : candidates)
        {
            // Sicherstellung, dass ObjA und B nicht die Selbe Speicheradresse haben also ein Objekt mit sich selbst
            // Die Kollision prüft. Falls doch, springen wir durch continue zum nächsten Kandidaten
            if (objA == objB) continue;

            // minmax 2 Werte der Speicheradresse und gibt diese so in er Reihenfolge A, B wieder.
            // Dies verhindert doppelte prüfungen da wir immer nur nach dieser Reihenfolge prüfen und so nie A, B
            // B, A entstehen kann, sondern immer das eindeutigige selbe Paar A, B
            auto pair = std::minmax(objA, objB);

            // count prüft die Liste ob das paar bereits existiert. Existiert dieses, so wird dieses Objekt übersprungen
            if (processed_pairs.count(pair)) continue;

            // Hier erfolgt der Collisioncheck. Ist dieser True wird an objA ein Pointer des objB übergeben und anderrum
            if (CheckCollisionRecs(objA->Get_Hitbox(), objB->Get_Hitbox()))
            {
                objA->On_Collision(objB);
                objB->On_Collision(objA);
            }
            // fügt das geprüfte Paar der insert Liste hinzu, welche im erneuten durchlauf in count geprüft wird
            processed_pairs.insert(pair);
        }
    }
}
