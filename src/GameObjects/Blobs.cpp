#include "GameObjects/Blobs.h"

#include "GameData.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

void Blobs::SetNum(int num)
{
    if(num < MIN_UNITS)
        mNum = MIN_UNITS;
    else if(num > MAX_UNITS)
        mNum = MAX_UNITS;
    else
        mNum = num;

    SetImage();
}

void Blobs::UpdateImage()
{
    SetImage();
}

void Blobs::SetImage()
{
    // set color
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // assign texture
    const int spriteId = SpriteCollectiblesId::BLOBS_1 + (mNum - 1);

    auto * tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteCollectiblesFile, spriteId);

    isoObj->SetTexture(tex);
}

} // namespace game
