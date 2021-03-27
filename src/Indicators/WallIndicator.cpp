#include "Indicators/WallIndicator.h"

#include "Player.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
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
    /*
    if(val)
    {
        if(nullptr == mTxtCost)
            CreateCostData();
    }
    else
    {
        delete mTxtCost;
        mTxtCost = nullptr;
    }
    */
}

void WallIndicator::Render()
{
    IsoObject::Render();

    /*
    if(mTxtCost)
    {
        // position label
        const int x = GetX() + (GetWidth() - mTxtCost->GetWidth()) * 0.5f;
        const int y = GetY() + (GetHeight() - mTxtCost->GetHeight()) * 0.5f;
        mTxtCost->SetPosition(x, y);

        mTxtCost->Render();
    }
    */
}

void WallIndicator::CreateCostData()
{
    /*
    using namespace lib::graphic;

    // create label
    FontManager * fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 14, Font::BOLD);

    delete mTxtCost;

    std::ostringstream s;
    s << std::fixed << std::setprecision(1) << mCost;

    mTxtCost = new Text(s.str().c_str(), font);
    mTxtCost->SetColor(mColorCost);
    */
}

void WallIndicator::UpdateImage()
{
    using namespace lib::graphic;

    std::cout << "WallIndicator::UpdateImage - faction: " << mFaction << " - block: " << mBlock << std::endl;

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
