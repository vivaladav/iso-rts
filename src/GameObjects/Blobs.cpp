#include "GameObjects/Blobs.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/UniformDistribution.h>

namespace game
{

const int MIN_UNITS = 1;
const int MAX_UNITS = 4;

Blobs::Blobs() : Collectable(GameObject::TYPE_BLOBS, 1, 1)
{
    sgl::utilities::UniformDistribution ran(MIN_UNITS, MAX_UNITS);
    mNum = ran.GetNextValue();

    SetObjColors();
    SetImage();
}

void Blobs::MaximizeUnits()
{
    mNum = MAX_UNITS;

    UpdateGraphics();
}

void Blobs::MinimizeUnits()
{
    mNum = MIN_UNITS;

    UpdateGraphics();
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
