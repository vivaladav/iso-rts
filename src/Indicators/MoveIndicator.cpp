#include "Indicators/MoveIndicator.h"

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

float const MoveIndicator::COST_UNKNOWN = -1.f;

MoveIndicator::MoveIndicator()
    : IsoObject(1, 1)
{
    using namespace lib::graphic;

    // indicator texture
    Texture * tex = TextureManager::Instance()->GetTexture("data/img/selection-f1.png");
    SetTexture(tex);

    // init cost label
    SetCost(0);
}

void MoveIndicator::SetCost(float val)
{
    using namespace lib::graphic;

    // (almost) same cost as current -> do nothing
    const float minDelta = 0.1f;
    const float valDiff = std::fabs(val - mCost);

    if(valDiff < minDelta)
        return ;

    // update cost value
    mCost = val;

    // create label
    FontManager * fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/OpenSans.ttf", 14, Font::BOLD);

    delete mTxtCost;

    const float minCost = 0.f;

    if(mCost < minCost)
        mTxtCost = new Text("?", font);
    else
    {
        std::ostringstream s;
        s << std::fixed << std::setprecision(1) << mCost;
        mTxtCost = new Text(s.str().c_str(), font);
    }
}

void MoveIndicator::Render()
{
    IsoObject::Render();

    // position label
    const int x = GetX() + (GetWidth() - mTxtCost->GetWidth()) * 0.5f;
    const int y = GetY() + (GetHeight() - mTxtCost->GetHeight()) * 0.5f;
    mTxtCost->SetPosition(x, y);

    mTxtCost->Render();
}

} // namespace game
