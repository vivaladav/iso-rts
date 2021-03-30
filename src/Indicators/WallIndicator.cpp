#include "Indicators/WallIndicator.h"

#include "Player.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>
#include <graphic/Text.h>

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include <iostream>

namespace game
{

WallIndicator::WallIndicator()
    : IsoObject(1, 1)
    , mFaction(NO_FACTION)
{
}

void WallIndicator::SetFaction(PlayerFaction faction)
{
    if(faction == mFaction)
        return ;

    mFaction = faction;

    UpdateImage();
}

void WallIndicator::SetBeforeAfterDirections(int br, int bc, int ar, int ac)
{
    // -1 direction away from the center
    // +1 direction toward the center
    // 0 no direction

    if(1 == bc)
    {
        if(1 == ar)
            mBlock = TOP_RIGHT;
        else if(-1 == ar)
            mBlock = BOTTOM_RIGHT;
        else
            mBlock = HORIZONTAL;
    }
    else if(-1 == bc)
    {
        if(1 == ar)
            mBlock = TOP_LEFT;
        else if(-1 == ar)
            mBlock = BOTTOM_LEFT;
        else
            mBlock = HORIZONTAL;
    }
    else if(1 == br)
    {
        if(1 == ac)
            mBlock = BOTTOM_LEFT;
        else if(-1 == ac)
            mBlock = BOTTOM_RIGHT;
        else
            mBlock = VERTICAL;
    }
    else if(-1 == br)
    {
        if(1 == ac)
            mBlock = TOP_LEFT;
        else if(-1 == ac)
            mBlock = TOP_RIGHT;
        else
            mBlock = VERTICAL;
    }
    else
    {
        if(1 == ar || -1 == ar)
            mBlock = VERTICAL;
        else if(1 == ac || -1 == ac)
            mBlock = HORIZONTAL;
        else
            mBlock = INVALID;
    }

    UpdateImage();
}

void WallIndicator::SetCost(float energy, float material)
{
    // update cost value
    mCostEnergy = energy;
    mCostMaterial = material;

    CreateCostData();
}

void WallIndicator::ShowCost(bool val)
{
    if(val)
    {
        if(nullptr == mTxtCostEnergy)
            CreateCostData();
    }
    else
    {
        delete mTxtCostEnergy;
        mTxtCostEnergy = nullptr;

        delete mIconEnergy;
        mIconEnergy = nullptr;

        delete mTxtCostMaterial;
        mTxtCostMaterial = nullptr;

        delete mIconMaterial;
        mIconMaterial = nullptr;
    }
}

void WallIndicator::Render()
{
    IsoObject::Render();

    if(mTxtCostEnergy)
    {
        const int iconMargin = 5;

        const int rowW1 = mTxtCostEnergy->GetWidth() + iconMargin + mIconEnergy->GetWidth();
        const int rowW2 = mTxtCostMaterial->GetWidth() + iconMargin + mIconMaterial->GetWidth();
        const int totH = mTxtCostEnergy->GetHeight() + mTxtCostMaterial->GetHeight();

        // ENERGY
        const int x1 = GetX() + (GetWidth() - rowW1) * 0.5f;
        const int y1 = GetY() + (GetHeight() - totH) * 0.5f;
        mTxtCostEnergy->SetPosition(x1, y1);

        const int icoX1 = x1 + mTxtCostEnergy->GetWidth() + iconMargin;
        const int icoY1 = y1 + (mTxtCostEnergy->GetHeight() - mIconEnergy->GetHeight()) * 0.5f;
        mIconEnergy->SetPosition(icoX1, icoY1);

        // MATERIAL
        const int x2 = GetX() + (GetWidth() - rowW2) * 0.5f;
        const int y2 = y1 + mTxtCostEnergy->GetHeight();
        mTxtCostMaterial->SetPosition(x2, y2);

        const int icoX2 = x2 + mTxtCostMaterial->GetWidth() + iconMargin;
        const int icoY2 = y2 + (mTxtCostMaterial->GetHeight() - mIconMaterial->GetHeight()) * 0.5f;
        mIconMaterial->SetPosition(icoX2, icoY2);

        mTxtCostEnergy->Render();
        mIconEnergy->Render();
        mTxtCostMaterial->Render();
        mIconMaterial->Render();
    }
}

void WallIndicator::CreateCostData()
{
    using namespace lib::graphic;

    FontManager * fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 14, Font::BOLD);

    // ENERGY
    delete mTxtCostEnergy;

    std::ostringstream s;
    s << std::fixed << std::setprecision(0) << std::roundf(mCostEnergy);

    mTxtCostEnergy = new Text(s.str().c_str(), font);
    mTxtCostEnergy->SetColor(mColorCost);

    if(nullptr == mIconEnergy)
        mIconEnergy = new Image("data/img/icon_energy_cost.png");

    // MATERIAL
    delete mTxtCostMaterial;

    s.clear();
    s.str(std::string());
    s << std::fixed << std::setprecision(0) << std::roundf(mCostMaterial);

    mTxtCostMaterial = new Text(s.str().c_str(), font);
    mTxtCostMaterial->SetColor(mColorCost);

    if(nullptr == mIconMaterial)
        mIconMaterial = new Image("data/img/icon_material_cost.png");
}

void WallIndicator::UpdateImage()
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    // not in a valid state
    if(NO_FACTION == mFaction || INVALID == mBlock)
    {
        SetTexture(tm->GetTexture("data/img/wall_plan_invalid.png"));
        return ;
    }

    // upate object body
    const char * files[] =
    {
        "data/img/wall_plan_h-f1.png",
        "data/img/wall_plan_v-f1.png",
        "data/img/wall_plan_tl-f1.png",
        "data/img/wall_plan_tr-f1.png",
        "data/img/wall_plan_bl-f1.png",
        "data/img/wall_plan_br-f1.png",

        "data/img/wall_plan_h-f2.png",
        "data/img/wall_plan_v-f2.png",
        "data/img/wall_plan_tl-f2.png",
        "data/img/wall_plan_tr-f2.png",
        "data/img/wall_plan_bl-f2.png",
        "data/img/wall_plan_br-f2.png",

        "data/img/wall_plan_h-f3.png",
        "data/img/wall_plan_v-f3.png",
        "data/img/wall_plan_tl-f3.png",
        "data/img/wall_plan_tr-f3.png",
        "data/img/wall_plan_bl-f3.png",
        "data/img/wall_plan_br-f3.png"
    };

    const unsigned int index = mFaction * NUM_BLOCKS + mBlock;
    SetTexture(tm->GetTexture(files[index]));
}

} // namespace game
