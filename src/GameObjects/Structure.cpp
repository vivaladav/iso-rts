#include "Structure.h"

#include "GameObject.h"

namespace game
{

GameObjectType Structure::StructureToGameObject(StructureType st)
{
    static const GameObjectType types[NUM_STRUCT_TYPES] =
    {
        OBJ_BASE,
        OBJ_DEF_TOWER,
        OBJ_RADAR_STATION,
        OBJ_RES_GEN_ENERGY,
        OBJ_RES_GEN_MATERIAL1,
        OBJ_RES_GEN_ENERGY_SOLAR,
        OBJ_RES_STORAGE_ENERGY,
        OBJ_RES_STORAGE_MATERIAL,
        OBJ_RES_STORAGE_DIAMONDS,
        OBJ_RES_STORAGE_BLOBS,
        OBJ_PRACTICE_TARGET,
        OBJ_WALL
    };

    return types[st];
}

} // namespace game
