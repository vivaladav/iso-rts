#pragma once

#include <vector>

namespace game
{

enum ObjClass : unsigned int
{
    // STRUCTURES
    OCS_BASE,
    OCS_RADAR,
    OCS_TARGET,
    OCS_TOWER,

    // UNITS
    OCU_GENERIC,
    OCU_SOLDIER,
    OCU_WORKER,

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

struct ObjectData
{
    ObjectData(const std::vector<int> & sts,
               const std::vector<int> & cst,
               const char * file, unsigned int texId,
               ObjClass oc, unsigned int type)
        : stats(sts)
        , costs(cst)
        , iconFile(file)
        , iconTexId(texId)
        , objClass(oc)
        , objType(type)
    {
    }

    std::vector<int> stats;
    std::vector<int> costs;

    const char * iconFile;
    unsigned int iconTexId;

    ObjClass objClass;

    unsigned int objType;

    static const char * STR_CLASS[NUM_OBJ_CLASSES];
    static const char * STR_STAT[NUM_TOT_OBJ_STATS];

    static const ObjectData NullObj;
};

} // namespace game
