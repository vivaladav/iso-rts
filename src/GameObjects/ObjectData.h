#pragma once

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
    OCS_WALL_GATE,

    // UNITS
    OCU_GENERIC,
    OCU_SCOUT,
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

enum ObjCategory : unsigned int
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

struct ObjectData
{
    ObjectData(const std::vector<int> & sts,
               const std::vector<int> & cst,
               const char * file, unsigned int texId,
               ObjClass oc, ObjCategory ocat, unsigned int type,
               unsigned int rs, unsigned int cs,
               const char * tit, const char * desc)
        : stats(sts)
        , costs(cst)
        , iconFile(file)
        , iconTexId(texId)
        , objClass(oc)
        , objCategory(ocat)
        , objType(type)
        , rows(rs)
        , cols(cs)
        , title(tit)
        , description(desc)
    {
    }

    std::vector<int> stats;
    std::vector<int> costs;

    const char * iconFile = nullptr;
    unsigned int iconTexId;

    ObjClass objClass;
    ObjCategory objCategory;
    unsigned int objType;

    unsigned int rows;
    unsigned int cols;

    const char * title = nullptr;
    const char * description = nullptr;

    static const char * STR_CLASS[NUM_OBJ_CLASSES];
    static const char * STR_STAT[NUM_TOT_OBJ_STATS];

    static const ObjectData NullObj;

    static const int MAX_STAT_VAL = 10;
};

} // namespace game
