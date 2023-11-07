#pragma once

#include "GameObjectTypes.h"

#include <vector>

namespace game
{

enum ObjClass : unsigned int
{
    // STRUCTURES
    OCS_BASE,
    OCS_GENERATOR,
    OCS_RADAR,
    OCS_STORAGE,
    OCS_TARGET,
    OCS_TOWER,
    OCS_WALL,
    OCS_WALL_GATE,

    // UNITS
    OCU_GENERIC,
    OCU_SCOUT,
    OCU_SOLDIER,
    OCU_WORKER,

    // OTHERS
    OCO_COLLECTABLE,
    OCO_SCENE,

    NUM_OBJ_CLASSES,

    OC_NULL
};

enum ObjStatId : unsigned int
{
    // GENERIC STATS
    OSTAT_ENERGY,
    OSTAT_VIEW_RADIUS,
    OSTAT_FIRE_RATE,
    OSTAT_FIRE_POWER,
    OSTAT_FIRE_ACCURACY,
    OSTAT_FIRE_RANGE,
    OSTAT_REGENERATION,
    OSTAT_RESISTANCE,
    OSTAT_SHIELD,

    NUM_GEN_OBJ_STATS,

    // UNIT ONLY
    OSTAT_SPEED = NUM_GEN_OBJ_STATS,
    OSTAT_CONSTRUCTION,
    OSTAT_CONQUEST,

    NUM_TOT_OBJ_STATS,
    NUM_UNIT_STATS = NUM_TOT_OBJ_STATS - NUM_GEN_OBJ_STATS
};

enum ObjFamily : unsigned int
{
    // STRUCTURE
    OCAT_GENERIC,
    OCAT_DEFENSE,
    OCAT_RESOURCES,
    OCAT_TECHNOLOGY,

    // UNIT
    OCAT_UNIT,

    NUM_OBJ_CATEGORIES,

    OCAT_UNDEFINED
};

struct ObjectBasicData
{
    ObjectBasicData(GameObjectTypeId type, ObjClass oc, ObjFamily ofam,
                    unsigned int rs, unsigned int cs)
        : objType(type)
        , objClass(oc)
        , objFamily(ofam)
        , rows(rs)
        , cols(cs)
    {
    }

    GameObjectTypeId objType;
    ObjClass objClass;
    ObjFamily objFamily;

    unsigned int rows;
    unsigned int cols;

    static const char * STR_CLASS[NUM_OBJ_CLASSES];

    static const ObjectBasicData NullObj;
};

struct ObjectFactionData
{
    ObjectFactionData(const std::vector<int> & sts,
                      const std::vector<int> & cst,
                      const char * file, unsigned int texId)
        : stats(sts)
        , costs(cst)
        , iconFile(file)
        , iconTexId(texId)
    {
    }

    std::vector<int> stats;
    std::vector<int> costs;

    const char * iconFile = nullptr;
    unsigned int iconTexId;

    static const char * STR_STAT[NUM_TOT_OBJ_STATS];

    static const ObjectFactionData NullObj;

    static const int MAX_STAT_VAL = 10;
};

} // namespace game
