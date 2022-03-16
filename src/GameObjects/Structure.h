#pragma once

#include "GameObject.h"

namespace game
{

enum StructureType : unsigned int
{
    STRUCT_BASE,
    STRUCT_DEF_TOWER,
    STRUCT_RADAR,
    STRUCT_RES_GEN_ENERGY,
    STRUCT_RES_GEN_MATERIAL,
    STRUCT_RES_GEN_SOLAR,
    STRUCT_RES_STO_ENERGY,
    STRUCT_RES_STO_MATERIAL,
    STRUCT_RES_STO_DIAMONDS,
    STRUCT_RES_STO_BLOBS,
    STRUCT_TARGET,
    STRUCT_WALL,
    STRUCT_WALL_GATE,

    NUM_STRUCT_TYPES,

    STRUCT_NULL
};

enum GameObjectType : unsigned int;

class Structure : public GameObject
{
public:
    Structure(GameObjectType type, int rows, int cols);

public:
    static GameObjectType StructureToGameObject(StructureType st);

    static const char * TITLES[NUM_STRUCT_TYPES];
    static const char * DESCRIPTIONS[NUM_STRUCT_TYPES];
};

inline Structure::Structure(GameObjectType type, int rows, int cols)
    : GameObject(type, rows, cols)
{
    SetStructure(true);
}

} // namespace game
