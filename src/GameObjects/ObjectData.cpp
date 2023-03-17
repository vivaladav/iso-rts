#include "ObjectData.h"

namespace game
{

const char * ObjectData::STR_CLASS[NUM_OBJ_CLASSES] =
{
    "BASE",
    "GENERATOR",
    "RADAR",
    "STORAGE",
    "TARGET",
    "TOWER",
    "GATE",

    "GENERIC",
    "SCOUT",
    "SOLDIER",
    "WORKER"
};

const char * ObjectData::STR_STAT[NUM_TOT_OBJ_STATS] =
{
    "ENERGY",
    "VIEW RADIUS",
    "FIRE RATE",
    "FIRE POWER",
    "FIRE ACCURACY",
    "FIRE RANGE",
    "REGENERATION",
    "RESISTANCE",
    "SHIELD",
    "SPEED",
    "CONSTRUCTION",
    "CONQUEST"
};

const ObjectData ObjectData::NullObj(std::vector<int>{}, std::vector<int>{}, nullptr, 0,
                                     OC_NULL, OCAT_UNDEFINED, -1, 0, 0, nullptr, nullptr);

} // namespace game
