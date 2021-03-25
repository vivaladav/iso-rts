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

    // upate object body
    const char * files[] =
    {
        "data/img/wall_plan_v-f1.png",
        "data/img/wall_plan_v-f2.png",
        "data/img/wall_plan_v-f3.png"
    };

    auto tm = TextureManager::Instance();
    // TODO proper image selection
    Texture * tex = tm->GetTexture(files[static_cast<unsigned int>(mFaction)]);

    SetTexture(tex);
}

} // namespace game
