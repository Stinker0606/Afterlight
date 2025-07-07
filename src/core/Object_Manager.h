#pragma once

#include <vector>
#include <algorithm>

#include "../game/Collidable.h"

class Object_Manager
{
public:
    //öffentliche Vektor, der alle vom Manager verwalteten Collidable-Objekte speichert.
    std::vector<Collidable*> managed_objects;

public:
    // Konstruktor des Object_Manager.
    Object_Manager();

    // Fügt ein Collidable-Objekt zur Liste Objekte hinzu.

    void AddObject(Collidable* object);

    // Entfernt ein bestimmtes Collidable-Objekt aus der Liste.
    void RemoveObject(Collidable* object);

    // Leert die gesamte Liste der verwalteten Objekte.
    void ClearAllObjects();

    void Cleanup_Objects();
};