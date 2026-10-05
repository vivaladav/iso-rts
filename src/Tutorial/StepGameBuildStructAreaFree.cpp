#include "Tutorial/StepGameBuildStructAreaFree.h"

#include "GameMap.h"
#include "IsoMap.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameBuildStructAreaFree::StepGameBuildStructAreaFree(const GameMap * gm, const IsoMap * isoMap,
                                                         const Cell2D & areaTL, const Cell2D & areaBR,
                                                         GameObjectTypeId type, unsigned int num, const char * text)
    : TutorialInfoStep(550, 130)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mGameMap(gm)
    , mAreaTL(areaTL)
    , mAreaBR(areaBR)
    , mType(type)
    , mNum(num)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetVisible(false);
    mFocusArea->SetCellArea(areaBR.row, areaBR.col, areaTL.row, areaTL.col, false);
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetBlinking(true);

    // CLICK FILTER
    auto cf = GetClickFilter();
    cf->ExpandClickableArea();

    // INFO
    if(text != nullptr)
        GetPanelInfo()->AddActionEntry(sm->GetCString(text), 0.f, false, false);
}

StepGameBuildStructAreaFree::~StepGameBuildStructAreaFree()
{
    delete mFocusArea;
}

void StepGameBuildStructAreaFree::OnStart()
{
    TutorialInfoStep::OnStart();

    mFocusArea->SetVisible(true);

    if(GetNumStructs() >= mNum)
        SetDone();
}

void StepGameBuildStructAreaFree::Update(float)
{
    if(GetNumStructs() >= mNum)
        SetDone();
}

unsigned int StepGameBuildStructAreaFree::GetNumStructs() const
{
    unsigned int num = 0;

    for(int r = mAreaTL.row; r <= mAreaBR.row; ++r)
    {
        const int ind0 = r * mGameMap->GetNumCols();

        for(int c = mAreaTL.col; c <= mAreaBR.col; ++c)
        {
            const int ind = ind0 + c;
            const auto obj = mGameMap->GetObject(ind);

            // make sure to count bigger objects only once
            if(obj != nullptr && obj->GetObjectType() == mType &&
               obj->GetRow0() == r && obj->GetCol0() == c)
                ++num;
        }
    }

    return num;
}

} // namespace game
