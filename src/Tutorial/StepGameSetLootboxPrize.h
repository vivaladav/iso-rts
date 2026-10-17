#pragma once

#include "Tutorial/TutorialStep.h"

namespace game
{

class LootBox;

enum ExtendedResource: unsigned int;

class StepGameSetLootboxPrize : public TutorialStep
{
public:
    StepGameSetLootboxPrize(LootBox * lb, ExtendedResource prize, int quantity);

    void OnStart() override;

private:
    LootBox * mLootbox = nullptr;

    ExtendedResource mPrize;
    unsigned int mQuantity;
};

} // namespace game
