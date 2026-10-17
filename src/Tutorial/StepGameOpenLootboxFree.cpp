#include "Tutorial/StepGameOpenLootboxFree.h"

#include "GameMap.h"
#include "IsoMap.h"
#include "GameObjects/LootBox.h"
#include "Widgets/Tutorial/FocusArea.h"
#include "Widgets/Tutorial/IsoFocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameOpenLootboxFree::StepGameOpenLootboxFree(const GameMap * gm, const GameObject * lootbox,
                                                 const IsoMap * isoMap, const char * intro,
                                                 const sgl::core::Pointd2D & p0)
    : TutorialInfoStep(600, 200)
    , mFocusArea(new FocusArea)
    , mIsoFocusArea(new IsoFocusArea(isoMap))
    , mLootbox(lootbox)
    , mGameMap(gm)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS AREAS
    const int padding = 5;
    const int objX = lootbox->GetX() - padding;
    const int objY = lootbox->GetY() - padding;
    const int objW = lootbox->GetWidth() + (padding * 2);
    const int objH = lootbox->GetHeight() + (padding * 2);

    mFocusArea->SetWorldArea(objX, objY, objW, objH, false);
    mFocusArea->SetCornersColorElement();
    mFocusArea->SetVisible(false);

    mIsoFocusArea->SetCell(mLootbox->GetRow0(), mLootbox->GetCol0());
    mIsoFocusArea->SetCornersColorAction();
    mIsoFocusArea->SetBlinking(true);
    mIsoFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();

    info->SetPosition(p0.x, p0.y);

    info->AddInfoEntry(sm->GetCString(intro), 7.f, true, false,
                       [this]
                       {
                            mFocusArea->SetVisible(true);
                       });

    info->AddActionEntry(sm->GetCString("TUT_GAME_LOOTBOX_2"), 0.f, false, false,
                        [this, isoMap]
                        {
                            // FOCUS AREAS
                            mFocusArea->SetVisible(false);
                            mIsoFocusArea->SetVisible(true);

                            // CLICK FILTER
                            GetClickFilter()->ExpandClickableArea();
                        });
}

StepGameOpenLootboxFree::~StepGameOpenLootboxFree()
{
    delete mFocusArea;
    delete mIsoFocusArea;
}

void StepGameOpenLootboxFree::Update(float)
{
    if(!mGameMap->HasObject(mLootbox))
        SetDone();
}

} // namespace game
