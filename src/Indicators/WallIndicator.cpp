#include "Indicators/WallIndicator.h"

#include "GameData.h"
#include "Player.h"
#include "GameObjects/GameObject.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>
#include <graphic/Text.h>

#include <string>

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

    UpdateCostColor();
}

void WallIndicator::SetBeforeAfterDirections(int br, int bc, int ar, int ac)
{
    // -1 direction away from the center
    // +1 direction toward the center
    // 0 no direction

    if(1 == bc)
    {
        if(1 == ar)
            mBlock = WB_TOP_RIGHT;
        else if(-1 == ar)
            mBlock = WB_BOTTOM_RIGHT;
        else
            mBlock = WB_HORIZONTAL;
    }
    else if(-1 == bc)
    {
        if(1 == ar)
            mBlock = WB_TOP_LEFT;
        else if(-1 == ar)
            mBlock = WB_BOTTOM_LEFT;
        else
            mBlock = WB_HORIZONTAL;
    }
    else if(1 == br)
    {
        if(1 == ac)
            mBlock = WB_BOTTOM_LEFT;
        else if(-1 == ac)
            mBlock = WB_BOTTOM_RIGHT;
        else
            mBlock = WB_VERTICAL;
    }
    else if(-1 == br)
    {
        if(1 == ac)
            mBlock = WB_TOP_LEFT;
        else if(-1 == ac)
            mBlock = WB_TOP_RIGHT;
        else
            mBlock = WB_VERTICAL;
    }
    else
    {
        if(1 == ar || -1 == ar)
            mBlock = WB_VERTICAL;
        else if(1 == ac || -1 == ac)
            mBlock = WB_HORIZONTAL;
        else
            mBlock = WB_INVALID;
    }

    UpdateImage();
}


GameObjectType WallIndicator::GetBlockType() const
{
    static const GameObjectType types[] =
    {
        OBJ_WALL_HORIZ,
        OBJ_WALL_VERT,
        OBJ_WALL_TL,
        OBJ_WALL_TR,
        OBJ_WALL_BL,
        OBJ_WALL_BR
    };

    if(WB_INVALID == mBlock)
        return OBJ_WALL_HORIZ;
    else
        return types[mBlock];
}

void WallIndicator::SetCost(unsigned int energy, unsigned int material)
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

    mTxtCostEnergy = new Text(std::to_string(mCostEnergy).c_str(), font);
    mTxtCostEnergy->SetColor(mColorCost);

    if(nullptr == mIconEnergy)
        mIconEnergy = new Image("data/img/icon_energy_cost.png");

    // MATERIAL
    delete mTxtCostMaterial;

    mTxtCostMaterial = new Text(std::to_string(mCostMaterial).c_str(), font);
    mTxtCostMaterial->SetColor(mColorCost);

    if(nullptr == mIconMaterial)
        mIconMaterial = new Image("data/img/icon_material_cost.png");
}

void WallIndicator::UpdateImage()
{
    using namespace lib::graphic;

    auto tm = TextureManager::Instance();

    // not in a valid state
    if(NO_FACTION == mFaction || WB_INVALID == mBlock)
    {
        SetTexture(tm->GetSprite(SpriteFileIndicators, IND_WB_INVALID));
        return ;
    }

    // upate object body
    const unsigned int index = IND_WB_FIRST + (mFaction * NUM_WALL_BLOCKS) + mBlock;
    SetTexture(tm->GetSprite(SpriteFileIndicators, index));
}

void WallIndicator::UpdateCostColor()
{
    if(FACTION_1 == mFaction)
        mColorCost = 0xf0dbdbff;
    else if(FACTION_2 == mFaction)
        mColorCost = 0xd9f2ddff;
    else
        mColorCost = 0xd6edf5ff;

    if(mTxtCostEnergy != nullptr)
    {
        mTxtCostEnergy->SetColor(mColorCost);
        mTxtCostMaterial->SetColor(mColorCost);
    }
}

} // namespace game
