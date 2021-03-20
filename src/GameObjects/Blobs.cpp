#include "GameObjects/Blobs.h"

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
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const char * imgFiles[] =
    {
        "data/img/blobs_1.png",
        "data/img/blobs_2.png",
        "data/img/blobs_3.png",
        "data/img/blobs_4.png"
    };

    tex = tm->GetTexture(imgFiles[mNum - 1]);

    isoObj->SetTexture(tex);
}

} // namespace game
