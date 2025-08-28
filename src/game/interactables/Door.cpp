#include "Door.h"

Door::Door(Rectangle hitbox, std::string target_map, std::string target_spawn_point)
    : target_map_(target_map), target_spawn_point_(target_spawn_point)
{
    this->hitbox = hitbox;
}

// Wir fügen einen neuen Kollisionstyp hinzu.
Collision_Type Door::Get_Collision_Type() const
{
    return Collision_Type::PORTAL;
}