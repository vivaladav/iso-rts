#include "Tutorial/StepGameSetCollectableUnits.h"

#include "GameMap.h"
#include "GameObjects/Collectable.h"

namespace game
{

StepGameSetCollectableUnits::StepGameSetCollectableUnits(const Cell2D & tl, const Cell2D & br,
                                                       const GameMap * gm, GameObjectTypeId type,
                                                       int min, int max)
    : mTL(tl)
    , mBR(br)
    , mGameMap(gm)
    , mGenType(type)
    , mMin(min)
    , mMax(max)
{
}

void StepGameSetCollectableUnits::OnStart()
{
    TutorialStep::OnStart();

    const unsigned int cols = mGameMap->GetNumCols();

    for(int r = mTL.row; r <= mBR.row; ++r)
    {
        const unsigned int ind0 = r * cols;

        for(int c = mTL.col; c <= mBR.col; ++c)
        {
            const unsigned int ind = ind0 + c;

            auto obj = mGameMap->GetObject(ind);

            if(obj != nullptr && obj->GetObjectType() == mGenType)
                static_cast<Collectable *>(obj)->RandomizeNumUnits(mMin, mMax);
        }
    }

    SetDone();
}

} // namespace game
