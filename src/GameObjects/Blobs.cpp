#include "GameObjects/Blobs.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

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

void Blobs::UpdateGraphics()
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

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteCollectiblesFile, spriteId);

    isoObj->SetTexture(tex);
}

void Blobs::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xf9b8f9ff);
    mObjColors.push_back(0xf589f5ff);
    mObjColors.push_back(0xe16be1ff);
    mObjColors.push_back(0xdb57dbff);
    mObjColors.push_back(0xd742d7ff);
    mObjColors.push_back(0xd22dd2ff);
}

} // namespace game
