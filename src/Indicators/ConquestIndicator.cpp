#include "Indicators/ConquestIndicator.h"

#include "GameData.h"
#include "Player.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/TextureManager.h>
#include <graphic/Text.h>

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace game
{

ConquestIndicator::ConquestIndicator()
    : IsoObject(1, 1)
{
    // init cost label
    SetCost(0);
}

void ConquestIndicator::SetFaction(PlayerFaction faction)
{
    using namespace lib::graphic;

    // upate object body
    const unsigned int indSprite = IND_CONQUEST_F1 + faction;

    auto tm = TextureManager::Instance();
    Texture * tex = tm->GetSprite(SpriteFileIndicators, indSprite);

    SetTexture(tex);

    // update cost label
    mColorCost = 0xE5E5E5FF;

    if(mTxtCost)
        mTxtCost->SetColor(mColorCost);
}

void ConquestIndicator::SetCost(float val)
{
    // (almost) same cost as current -> do nothing
    const float minDelta = 0.1f;
    const float valDiff = std::fabs(val - mCost);

    if(valDiff < minDelta)
        return ;

    // update cost value
    mCost = val;

    CreateLabelCost();
}

void ConquestIndicator::ShowCost(bool val)
{
    if(val)
    {
        if(nullptr == mTxtCost)
            CreateLabelCost();
    }
    else
    {
        delete mTxtCost;
        mTxtCost = nullptr;
    }
}

void ConquestIndicator::Render()
{
    IsoObject::Render();

    if(mTxtCost)
    {
        // position label
        const int x = GetX() + (GetWidth() - mTxtCost->GetWidth()) * 0.5f;
        const int y = GetY() + (GetHeight() - mTxtCost->GetHeight()) * 0.5f;
        mTxtCost->SetPosition(x, y);

        mTxtCost->Render();
    }
}

void ConquestIndicator::CreateLabelCost()
{
    using namespace lib::graphic;

    // create label
    FontManager * fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 14, Font::BOLD);

    delete mTxtCost;

    std::ostringstream s;
    s << std::fixed << std::setprecision(1) << mCost;

    mTxtCost = new Text(s.str().c_str(), font);
    mTxtCost->SetColor(mColorCost);
}

} // namespace game
