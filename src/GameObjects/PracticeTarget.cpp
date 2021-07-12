#include "GameObjects/PracticeTarget.h"

#include "GameData.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

PracticeTarget::PracticeTarget(int rows, int cols)
    : GameObject(GameObjectType::OBJ_PRACTICE_TARGET, rows, cols)
{
    SetStructure(true);

    SetImage();
}

void PracticeTarget::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void PracticeTarget::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // set texture
    const unsigned int spriteId = SpriteIdSceneElements::SPR_PRACTICE_TARGET1;
    auto tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileSceneElements, spriteId);
    isoObj->SetTexture(tex);
}

void PracticeTarget::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xb2b2b2ff);
    mObjColors.push_back(0xc06158ff);
    mObjColors.push_back(0xa7483fff);
    mObjColors.push_back(0x74423fff);
    mObjColors.push_back(0x603c39ff);
    mObjColors.push_back(0x503230ff);
}

} // namespace game
