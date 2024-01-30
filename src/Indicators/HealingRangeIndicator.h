#pragma once

#include "IsoObject.h"

namespace game
{

enum PlayerFaction : unsigned int;

class HealingRangeIndicator : public IsoObject
{
public:
    HealingRangeIndicator();

    void SetFaction(PlayerFaction faction);
};

} // namespace game
