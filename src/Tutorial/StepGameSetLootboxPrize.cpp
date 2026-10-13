#include "Tutorial/StepGameSetLootboxPrize.h"

#include "GameObjects/LootBox.h"

namespace game
{

StepGameSetLootboxPrize::StepGameSetLootboxPrize(LootBox * lb, ExtendedResource prize, int quantity)
    : mLootbox(lb)
    , mPrize(prize)
    , mQuantity(quantity)
{
}

void StepGameSetLootboxPrize::OnStart()
{
    TutorialStep::OnStart();

    mLootbox->mPrizeType = mPrize;
    mLootbox->mPrizeQuantity = mQuantity;

    SetDone();
}

} // namespace game
