#pragma once

#include "IsoObject.h"

namespace game
{

enum PlayerFaction : unsigned int;

class AttackRangeIndicator : public IsoObject
{
public:
    AttackRangeIndicator();

    void SetFaction(PlayerFaction faction);
};

} // namespace game
