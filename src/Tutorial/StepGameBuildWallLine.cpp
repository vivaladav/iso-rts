#include "Tutorial/StepGameBuildWallLine.h"

#include "GameMap.h"
#include "IsoMap.h"
#include "GameObjects/GameObject.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameBuildWallLine::StepGameBuildWallLine(const GameMap * gm, const IsoMap * im,
                                     const Cell2D & areaTL, const Cell2D & areaBR,
                                     const char * text)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(im))
    , mGameMap(gm)
    , mAreaTL(areaTL)
    , mAreaBR(areaBR)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetCellArea(areaBR.row, areaBR.col, areaTL.row, areaTL.col, true);
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false, [this]
                        {
                            mFocusArea->SetVisible(true);

                            auto cf = GetClickFilter();
                            cf->ExpandClickableArea();
                        });
}

StepGameBuildWallLine::~StepGameBuildWallLine()
{
    delete mFocusArea;
}

void StepGameBuildWallLine::Update(float)
{
    // cycle along cols
    if(mAreaBR.row == mAreaTL.row)
    {
        const int ind0 = mAreaBR.row * mGameMap->GetNumCols();

        for(int c = mAreaTL.col; c <= mAreaBR.col; ++c)
        {
            const int ind = ind0 + c;

            const auto obj = mGameMap->GetObject(ind);

            if(obj == nullptr || !obj->IsStructure())
                return ;
        }
    }
    // cycle along rows
    else if(mAreaBR.col == mAreaTL.col)
    {
        for(int r = mAreaTL.row; r <= mAreaBR.row; ++r)
        {
            const auto obj = mGameMap->GetObject(r, mAreaTL.col);

            if(obj == nullptr || !obj->IsStructure())
                return ;
        }
    }

    SetDone();
}

} // namespace game
