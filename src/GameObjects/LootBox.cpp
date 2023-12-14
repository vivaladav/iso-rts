#include "GameObjects/LootBox.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

LootBox::LootBox()
    : Collectable(GameObject::TYPE_LOOTBOX, 1, 1)
{
    SetImage();

    SetObjColors();
}

void LootBox::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void LootBox::SetImage()
{
    auto * tm = sgl::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const unsigned int sel = static_cast<unsigned int>(IsSelected());
    const unsigned int texInd = IND_LOOTBOX + sel;

    sgl::graphic::Texture * tex = tm->GetSprite(SpriteCollectiblesFile, texInd);
    isoObj->SetTexture(tex);
}

void LootBox::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xdba457ff);
    mObjColors.push_back(0xd28d2dff);
    mObjColors.push_back(0xe0b06cff);
    mObjColors.push_back(0x805519ff);
    mObjColors.push_back(0x6b4715ff);
    mObjColors.push_back(0x95631dff);
}

} // namespace game
