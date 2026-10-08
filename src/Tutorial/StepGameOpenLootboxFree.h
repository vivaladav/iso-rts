#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class FocusArea;
class GameMap;
class GameObject;
class IsoFocusArea;
class IsoMap;
class Player;

class StepGameOpenLootboxFree : public TutorialInfoStep
{
public:
    StepGameOpenLootboxFree(const GameMap * gm, const GameObject * lootbox, const IsoMap * isoMap,
                            const char * intro, const sgl::core::Pointd2D & p0);
    ~StepGameOpenLootboxFree();

    void Update(float delta) override;

private:
    FocusArea * mFocusArea = nullptr;
    IsoFocusArea * mIsoFocusArea = nullptr;

    const GameObject * mLootbox = nullptr;
    const GameMap * mGameMap = nullptr;
};

} // namespace game
