#pragma once

#include <vector>
#include <algorithm>

#include "../game/Collidable.h"
#include <memory>


class Object_Manager
{
public:
    //öffentliche Vektor, der alle vom Manager verwalteten Collidable-Objekte speichert.
    std::vector<std::shared_ptr<Collidable>> managed_objects;

public:
    // Konstruktor des Object_Manager.
    Object_Manager();

    // Fügt ein Collidable-Objekt zur Liste Objekte hinzu.

    void AddObject(std::shared_ptr<Collidable> object);

    // Entfernt ein bestimmtes Collidable-Objekt aus der Liste.
    void RemoveObject(std::shared_ptr<Collidable> object);

    // Leert die gesamte Liste der verwalteten Objekte.
    void ClearAllObjects();

    void Cleanup_Objects();


};