#include "Tutorial/StepGameConquerCity.h"

#include "IsoMap.h"
#include "GameObjects/CityGroup.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameConquerCity::StepGameConquerCity(const CityGroup * cg, const IsoMap * isoMap,
                                         const char * text, const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(isoMap))
    , mCity(cg)
{
    auto sm = sgl::utilities::StringManager::Instance();

    Cell2D cityTL;
    Cell2D cityBR;
    cg->GetCityBoundaries(cityTL, cityBR);

    // FOCUS
    mFocusArea->SetVisible(false);
    mFocusArea->SetCellArea(cityBR.row, cityBR.col, cityTL.row, cityTL.col);
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetBlinking(true);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(text), 0.f, false, false,
                        [this]
                        {
                            // FOCUS
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            GetClickFilter()->SetEnabled(false);
                        });
}

StepGameConquerCity::~StepGameConquerCity()
{
    delete mFocusArea;
}

void StepGameConquerCity::Update(float)
{
    if(mCity->IsCityConquered())
        SetDone();
}

} // namespace game
