#include "Tutorial/StepGameDialogTradingBuy.h"

#include "Game.h"
#include "GameConstants.h"
#include "Player.h"
#include "Widgets/DialogTrading.h"
#include "Widgets/Tutorial/FocusArea.h"
#include "Widgets/Tutorial/PanelClickFilter.h"
#include "Widgets/Tutorial/PanelInfoTutorial.h"

#include <sgl/sgui/AbstractButton.h>
#include <sgl/sgui/Slider.h>
#include <sgl/sgui/Stage.h>
#include <sgl/utilities/StringManager.h>

namespace game
{

StepGameDialogTradingBuy::StepGameDialogTradingBuy(const Game * game, const Player * p,
                                                   const DialogTrading * dialog)
    : TutorialInfoStep(600, 150)
    , mFocusArea(new FocusArea)
    , mDialog(dialog)
    , mGame(game)
    , mPlayer(p)
{
    auto sm = sgl::utilities::StringManager::Instance();

    // FOCUS
    mFocusArea->SetCornersColorAction();
    mFocusArea->SetBlinking(true);
    mFocusArea->SetVisible(false);

    // INFO
    auto info = GetPanelInfo();
    info->SetPosition(650, 100);

    // DEFINE QUANTITIES TO BUY
    int spent = 0;

    const unsigned int maxMaterial = 100;
    const unsigned int valMaterial = GetQuantityToBuy(ER_MATERIAL, maxMaterial, spent);
    spent += valMaterial * mGame->GetResourcePriceBuy(ER_MATERIAL);

    const unsigned int maxEnergy = 30;
    const unsigned int valEnergy = GetQuantityToBuy(ER_ENERGY, maxEnergy, spent);
    spent += valEnergy * mGame->GetResourcePriceBuy(ER_ENERGY);

    const unsigned int maxResearch = 10;
    const unsigned int valResearch = GetQuantityToBuy(ER_RESEARCH, maxResearch, spent);
    spent += valResearch * mGame->GetResourcePriceBuy(ER_RESEARCH);

    bool buying = false;

    if(valEnergy > 0)
    {
        const std::string strEnergy = sm->GetParametricString("TUT_GAME_TRAD_POST_4a",
                                                              std::to_string(valEnergy));
        info->AddActionEntry(strEnergy.c_str(), 0.f, false, true,
                            [this, valEnergy]
                            {
                                HandleButton(DialogTrading::TR_ENERGY, valEnergy);
                            });

        buying = true;
    }

    if(valMaterial > 0)
    {
        const std::string strMaterial = sm->GetParametricString("TUT_GAME_TRAD_POST_4b",
                                                                std::to_string(valMaterial));
        info->AddActionEntry(strMaterial.c_str(), 0.f, false, true,
                             [this, valMaterial]
                             {
                                HandleButton(DialogTrading::TR_MATERIAL, valMaterial);
                             });

        buying = true;
    }

    if(valResearch > 0)
    {
        const std::string strResearch = sm->GetParametricString("TUT_GAME_TRAD_POST_4c",
                                                             std::to_string(valResearch));
        info->AddActionEntry(strResearch.c_str(), 0.f, false, true,
                             [this, valResearch]
                             {
                                HandleButton(DialogTrading::TR_RESEARCH, valResearch);
                             });

        buying = true;
    }

    // this should always happen
    if(buying)
    {
        info->AddActionEntry(sm->GetCString("TUT_GAME_TRAD_POST_3"), 0.f, false, true,
                        [this, info]
                         {
                            auto btn = mDialog->mButtonBuy;
                            const auto cid = btn->AddOnClickFunction([this]
                                                    {
                                                        GetPanelInfo()->Continue();
                                                    });

                            mCallbacks.emplace(btn, cid);

                            // FOCUS
                            const int padding = 10;
                            const int fX = btn->GetScreenX() - padding;
                            const int fY = btn->GetScreenY() - padding;
                            const int fW = btn->GetWidth() + (padding * 2);
                            const int fH = btn->GetHeight() + (padding * 2);

                            mFocusArea->SetScreenArea(fX, fY, fW, fH, true);
                            mFocusArea->SetVisible(true);

                            // CLICK FILTER
                            const int fX2 = btn->GetScreenX();
                            const int fY2 = btn->GetScreenY();
                            const int fW2 = btn->GetWidth();
                            const int fH2 = btn->GetHeight();
                            GetClickFilter()->SetScreenClickableArea(fX2, fY2, fW2, fH2);
                         });
    }

    info->AddActionEntry(sm->GetCString("TUT_GAME_MISSION_GOALS_DIALOG_5"), 0.f, false, false,
                         [this]
                         {
                            auto btn = mDialog->mButtonClose;

                            const auto cid = btn->AddOnClickFunction([this]
                                                    {
                                                        SetDone();
                                                    });

                            mCallbacks.emplace(btn, cid);

                            const int padding = 10;
                            const int x = btn->GetScreenX() - padding;
                            const int y = btn->GetScreenY() - padding;
                            const int w = btn->GetWidth() + (2 * padding);
                            const int h = btn->GetHeight() + (2 * padding);

                            GetClickFilter()->SetScreenClickableArea(x, y, w, h);

                            mFocusArea->SetScreenArea(x, y, w, h, true);
                            mFocusArea->SetCornersColorAction();
                            mFocusArea->SetBlinking(true);
                            mFocusArea->SetVisible(true);
                         });
}

StepGameDialogTradingBuy::~StepGameDialogTradingBuy()
{
    delete mFocusArea;

    // clear callbacks
    for(auto it : mCallbacks)
        (it.first)->RemoveClickFunction(it.second);
}

void StepGameDialogTradingBuy::OnStart()
{
    TutorialInfoStep::OnStart();

    // move elements to front
    auto stage = sgl::sgui::Stage::Instance();
    stage->MoveChildToFront(mFocusArea);
    stage->MoveChildToFront(GetClickFilter());
    stage->MoveChildToFront(GetPanelInfo());
}

unsigned int StepGameDialogTradingBuy::GetQuantityToBuy(ExtendedResource res, unsigned int max,
                                                        int spent) const
{
    const int money = mPlayer->GetResource(ER_MONEY).GetValue() - spent;
    const int price = mGame->GetResourcePriceBuy(res);
    const unsigned int maxCanAfford = money / price;

    if(maxCanAfford > max)
        return max;
    else
        return maxCanAfford - (maxCanAfford % 10);
}

void StepGameDialogTradingBuy::HandleButton(unsigned int res, unsigned int value)
{
    auto btn = mDialog->mButtonsBuyPlus[res];
    const auto cid = btn->AddOnClickFunction([this, res, value]
                                {
                                    if(mDialog->mBuy[res] == value)
                                        GetPanelInfo()->Continue();
                                });

    mCallbacks.emplace(btn, cid);

    // FOCUS
    const int padding = 10;
    const int fX = btn->GetScreenX() - padding;
    const int fY = btn->GetScreenY() - padding;
    const int fW = btn->GetWidth() + (padding * 2);
    const int fH = btn->GetHeight() + (padding * 2);

    mFocusArea->SetScreenArea(fX, fY, fW, fH, true);
    mFocusArea->SetVisible(true);

    // CLICK FILTER
    const int fX2 = btn->GetScreenX();
    const int fY2 = btn->GetScreenY();
    const int fW2 = btn->GetWidth();
    const int fH2 = btn->GetHeight();
    GetClickFilter()->SetScreenClickableArea(fX2, fY2, fW2, fH2);
}

} // namespace game
