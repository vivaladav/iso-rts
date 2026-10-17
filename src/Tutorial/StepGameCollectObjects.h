#pragma once

#include "Cell2D.h"
#include "GameObjects/GameObjectTypes.h"
#include "Tutorial/TutorialInfoStep.h"

#include <sgl/core/Point.h>

namespace game
{

class GameMap;
class IsoFocusArea;
class IsoMap;

class StepGameCollectObjects : public TutorialInfoStep
{
public:
    StepGameCollectObjects(GameObjectTypeId objType, const GameMap * gm, const IsoMap * im,
                           const Cell2D & areaTL, const Cell2D & areaBR,
                           const char * intro, const sgl::core::Pointd2D & p0);
    ~StepGameCollectObjects();

    void Update(float delta) override;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const Cell2D mAreaTL;
    const Cell2D mAreaBR;

    const GameMap * mGameMap = nullptr;
    GameObjectTypeId mObjType;
};

} // namespace game
