#include "ObjectData.h"

namespace game
{

const char * ObjectData::STR_CLASS[NUM_OBJ_CLASSES] =
{
    "BASE",
    "RADAR",
    "TARGET",
    "TOWER",

    "GENERIC",
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
    "REGENERATION",
    "RESISTANCE",
    "SHIELD",
    "SPEED",
    "CONSTRUCTION",
    "CONQUEST"
};

} // namespace game
