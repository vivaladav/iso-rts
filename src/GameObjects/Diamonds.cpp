#include "GameObjects/Diamonds.h"

#include "GameData.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

void Diamonds::SetNum(int num)
{
    if(num < MIN_UNITS)
        mNum = MIN_UNITS;
    else if(num > MAX_UNITS)
        mNum = MAX_UNITS;
    else
        mNum = num;

    SetImage();
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

    auto * tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteCollectiblesFile, spriteId);

    isoObj->SetTexture(tex);
}

} // namespace game
