#include "GameObjects/Temple.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

Temple::Temple()
    : Structure(GameObject::TYPE_TEMPLE, GameObject::CAT_GENERIC, 3, 3)
{
    SetCanBeConquered(true);

    SetVisibilityLevel(4);
}

void Temple::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void Temple::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const unsigned int faction = GetFaction();
    const unsigned int sel = static_cast<unsigned int>(IsSelected());

    unsigned int texInd;

    if(NO_FACTION == faction)
        texInd = ID_STRUCT_TEMPLE + sel;
    else
        texInd = ID_STRUCT_TEMPLE_F1 + (faction * NUM_TEMPLE_SPRITES_PER_FAC) + sel;

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);

    isoObj->SetTexture(tex);
}

void Temple::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xbaa35eff);
    mObjColors.push_back(0xa18245ff);
    mObjColors.push_back(0x7d6535ff);
    mObjColors.push_back(0xa07c45ff);
    mObjColors.push_back(0x8f743dff);
    mObjColors.push_back(0x745e32ff);
}

} // namespace game
