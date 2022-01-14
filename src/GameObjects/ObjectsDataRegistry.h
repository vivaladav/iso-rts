#pragma once

#include "GameConstants.h"
#include "ObjectData.h"

#include <array>
#include <vector>

namespace game
{

class ObjectsDataRegistry
{
public:
    ObjectsDataRegistry();

    const std::vector<ObjectData> & GetStructures(PlayerFaction f) const;
    const std::vector<ObjectData> & GetUnits(PlayerFaction f) const;

private:
    void InitStructures();
    void InitUnits();

private:
    std::array<std::vector<ObjectData>, NUM_FACTIONS> mStructures;
    std::array<std::vector<ObjectData>, NUM_FACTIONS> mUnits;
};

inline const std::vector<ObjectData> & ObjectsDataRegistry::GetStructures(PlayerFaction f) const
{
    return mStructures[f];
}

inline const std::vector<ObjectData> & ObjectsDataRegistry::GetUnits(PlayerFaction f) const
{
    return mUnits[f];
}

} // namespace game
