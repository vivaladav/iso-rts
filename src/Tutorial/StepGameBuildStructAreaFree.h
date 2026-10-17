#pragma once

#include "Cell2D.h"
#include "GameObjects/GameObjectTypes.h"
#include "Tutorial/TutorialInfoStep.h"

namespace game
{

class GameMap;
class IsoFocusArea;
class IsoMap;

class StepGameBuildStructAreaFree : public TutorialInfoStep
{
public:
    StepGameBuildStructAreaFree(const GameMap * gm, const IsoMap * isoMap, const Cell2D & areaTL,
                                const Cell2D & areaBR, GameObjectTypeId type, unsigned int num,
                                const char * text);
    ~StepGameBuildStructAreaFree();

    void OnStart() override;

    void Update(float delta) override;

private:
    unsigned int GetNumStructs() const;

private:
    IsoFocusArea * mFocusArea = nullptr;

    const GameMap * mGameMap = nullptr;

    const Cell2D mAreaTL;
    const Cell2D mAreaBR;

    const GameObjectTypeId mType;

    const unsigned int mNum = 0;
};

} // namespace game
