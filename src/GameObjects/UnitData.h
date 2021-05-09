#pragma once

namespace game
{

enum UnitType : unsigned int;

enum UnitStats : unsigned int
{
    USTAT_ENERGY,
    USTAT_RESISTANCE,
    USTAT_ATTACK,
    USTAT_SPEED,
    USTAT_CONSTRUCTION,
    USTAT_CONQUEST
};

struct UnitData
{
    UnitData(UnitType t, int stEne, int stRes, int stAtt, int stSpe,
             int stCons, int stConq, int costEne, int costMat);

    UnitType type;

    int statEnergy;
    int statResistance;
    int statAttack;
    int statSpeed;
    int statConstruction;
    int statConquest;

    int costEnergy;
    int costMaterial;
};

inline UnitData::UnitData(UnitType t, int stEne, int stRes, int stAtt, int stSpe,
                          int stCons, int stConq, int costEne, int costMat)
    : type(t)
    , statEnergy(stEne)
    , statResistance(stRes)
    , statAttack(stAtt)
    , statSpeed(stSpe)
    , statConstruction(stCons)
    , statConquest(stConq)
    , costEnergy(costEne)
    , costMaterial(costMat)
{
}

} // namespace game
