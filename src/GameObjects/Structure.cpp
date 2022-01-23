#include "Structure.h"

#include "GameObject.h"

namespace game
{

const char * Structure::TITLES[NUM_STRUCT_TYPES] =
{
    "BASE",
    "DEFENSIVE TOWER",
    "RADAR STATION",
    "ENERGY GENERATOR",
    "MATERIAL GENERATOR",
    "SOLAR PANELS",
    "ENERGY STORAGE",
    "MATERIAL STORAGE",
    "DIAMONDS STORAGE",
    "BLOBS STORAGE",
    "PRACTICE TARGET",
    "WALL"
};

const char * Structure::DESCRIPTIONS[NUM_STRUCT_TYPES] =
{
    "A control center. It can create units. You need to defend it if you don't want to lose a territory.",
    "A basic defensive tower.",
    "A radar station that can show a big portion of the map.",
    "A generator of energy.\nConnect it to your base to produce energy.",
    "A generator of material.\n Connect it to your base to mine material.",
    "A solar panel.\nConnect it to your base to produce energy. ",
    "Storage unit that can contain 500 units of energy.",
    "Storage unit that can contain 250 units of material.",
    "Storage unit that can contain 150 units of diamonds.",
    "Storage unit that can contain 100 units of blobs.",
    "A practice target.\nIt can be used to train your units and to improve their attack skills.",
    "A defensive wall."
};

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
