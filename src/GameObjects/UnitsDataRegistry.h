#pragma once

#include "GameObjects/UnitData.h"

#include <vector>

namespace game
{

class UnitsDataRegistry
{
public:
    UnitsDataRegistry();

    const UnitData & GetData(UnitType type) const;

private:
    std::vector<UnitData> mData;
};

inline const UnitData & UnitsDataRegistry::GetData(UnitType type) const
{
    return mData[type];
}

} // namespace game
