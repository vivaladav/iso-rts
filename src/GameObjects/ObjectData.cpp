#include "ObjectData.h"

#include "GameObjects/GameObject.h"

namespace game
{

const char * ObjectBasicData::STR_CLASS[NUM_OBJ_CLASSES] =
{
    "BASE",
    "GENERATOR",
    "RADAR",
    "RELICS",
    "STORAGE",
    "TARGET",
    "TOWER",
    "WALL",
    "GATE",

    "GENERIC",
    "MEDIC",
    "SCOUT",
    "SOLDIER",
    "WORKER",

    "COLLECTABLE",
    "SCENE"
};

const char * ObjectFactionData::STR_STAT[NUM_TOT_OBJ_STATS] =
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
    "CONQUEST",
    "HEALING",
};

const ObjectBasicData ObjectBasicData::NullObj(GameObject::TYPE_NULL, OC_NULL,
                                               OCAT_UNDEFINED, nullptr, 0, 0, 0);

const ObjectFactionData ObjectFactionData::NullObj({}, {}, nullptr, 0);

} // namespace game
