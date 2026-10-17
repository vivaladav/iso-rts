#include "Tutorial/StepGameCollectObjects.h"

#include "GameMap.h"
#include "IsoMap.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameCollectObjects::StepGameCollectObjects(GameObjectTypeId objType,
                                               const GameMap * gm, const IsoMap * im,
                                               const Cell2D & areaTL, const Cell2D & areaBR,
                                               const char * intro, const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(550, 150)
    , mFocusArea(new IsoFocusArea(im))
    , mAreaTL(areaTL)
    , mAreaBR(areaBR)
    , mGameMap(gm)
    , mObjType(objType)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetCellArea(areaBR.row, areaBR.col, areaTL.row, areaTL.col, true);
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddActionEntry(sm->GetCString(intro), 0.f, false, false, [this, im]
                        {
                            mFocusArea->SetVisible(true);

                            auto cf = GetClickFilter();
                            cf->ExpandClickableArea();
                        });
}

StepGameCollectObjects::~StepGameCollectObjects()
{
    delete mFocusArea;
}

void StepGameCollectObjects::Update(float)
{
    std::vector<GameObject *> objs;
    mGameMap->GetObjectsInAreaByType(objs, mAreaTL, mAreaBR, mObjType);

    if(objs.empty())
        SetDone();
}

} // namespace game
