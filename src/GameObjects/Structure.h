#pragma once

#include "GameObject.h"

namespace game
{

enum StructureType : unsigned int
{
    STRUCT_BASE,
    STRUCT_DEF_TOWER,
    STRUCT_RADAR,
    STRUCT_RES_GENERATOR,
    STRUCT_TARGET,
    STRUCT_WALL,

    NUM_STRUCT_TYPES,

    STRUCT_NULL
};

class Structure : public GameObject
{
public:
    Structure(GameObjectType type, int rows, int cols);
};

inline Structure::Structure(GameObjectType type, int rows, int cols)
    : GameObject(type, rows, cols)
{
    SetStructure(true);
}

} // namespace game
