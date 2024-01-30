#include "GameObjects/Diamonds.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/UniformDistribution.h>

namespace game
{

const int MIN_UNITS = 1;
const int MAX_UNITS = 4;

Diamonds::Diamonds() : Collectable(GameObject::TYPE_DIAMONDS, 1, 1)
{
    sgl::utilities::UniformDistribution ran(MIN_UNITS, MAX_UNITS);
    mNum = ran.GetNextValue();

    SetObjColors();
    SetImage();
}

void Diamonds::MaximizeUnits()
{
    mNum = MAX_UNITS;

    UpdateGraphics();
}

void Diamonds::MinimizeUnits()
{
    mNum = MIN_UNITS;

    UpdateGraphics();
}

void Diamonds::UpdateGraphics()
{
    SetImage();
}

void Diamonds::SetImage()
{
    // set color
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // assign texture
    const int spriteId = SpriteCollectiblesId::DIAMONDS_1 + (mNum - 1);

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteCollectiblesFile, spriteId);

    isoObj->SetTexture(tex);
}

void Diamonds::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xf9f4b8ff);
    mObjColors.push_back(0xf5ed89ff);
    mObjColors.push_back(0xe1d66bff);
    mObjColors.push_back(0xdbd057ff);
    mObjColors.push_back(0xd7ca42ff);
    mObjColors.push_back(0xbdb128ff);
}

} // namespace game
