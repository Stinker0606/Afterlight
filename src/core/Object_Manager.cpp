//
// Created by Kruse on 02/07/2025.
//

#include "Object_Manager.h"


#include <iostream>

// Implementierung des Konstruktors.
Object_Manager::Object_Manager()
{

}

void Object_Manager::AddObject(Collidable* object)
{
    managed_objects.push_back(object);

}

void Object_Manager::RemoveObject(Collidable* object)
{
    managed_objects.erase(std::remove(managed_objects.begin(), managed_objects.end(), object), managed_objects.end());
}


void Object_Manager::ClearAllObjects()
{
    managed_objects.clear();

}

void Object_Manager::Cleanup_Objects()
{
    auto new_end = std::remove_if(managed_objects.begin(), managed_objects.end(),
                                  [](Collidable* obj)
                                  {
                                      if (obj->Is_Marked_For_Destruction())
                                      {
                                          delete obj;
                                          return true;
                                      }
                                      return false;
                                  });
    managed_objects.erase(new_end, managed_objects.end());
}