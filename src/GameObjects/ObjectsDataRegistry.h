#pragma once

#include "GameConstants.h"
#include "GameObjects/GameObjectTypes.h"
#include "ObjectData.h"

#include <array>
#include <unordered_map>

namespace game
{

class ObjectsDataRegistry
{
public:
    ObjectsDataRegistry();

    const ObjectBasicData & GetObjectData(GameObjectTypeId type) const;
    const ObjectFactionData & GetFactionData(PlayerFaction f, GameObjectTypeId type) const;

private:
    void InitObjectData();
    void InitFactionData();

private:
    std::unordered_map<GameObjectTypeId, ObjectBasicData> mData;
    std::array<std::unordered_map<GameObjectTypeId, ObjectFactionData>, NUM_FACTIONS> mFactionData;
};

} // namespace game
