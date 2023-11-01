#pragma once

#include "GameConstants.h"
#include "GameObjects/GameObjectTypes.h"
#include "ObjectData.h"

#include <array>
#include <unordered_map>

namespace game
{

enum UnitType : unsigned int;

class ObjectsDataRegistry
{
public:
    ObjectsDataRegistry();

    const ObjectData & GetStructure(PlayerFaction f, GameObjectTypeId type) const;
    const ObjectData & GetUnit(PlayerFaction f, UnitType type) const;

private:
    void InitStructures();
    void InitUnits();

private:
    std::array<std::unordered_map<GameObjectTypeId, ObjectData>, NUM_FACTIONS> mStructures;
    std::array<std::unordered_map<UnitType, ObjectData>, NUM_FACTIONS> mUnits;
};

} // namespace game
