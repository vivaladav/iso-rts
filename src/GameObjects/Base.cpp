#include "GameObjects/Base.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

Base::Base()
    : Structure(GameObject::TYPE_BASE, GameObject::CAT_GENERIC, 3, 3)
{
    SetVisibilityLevel(6);
    // base is linked to itself
    SetLinked(true);
}

void Base::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void Base::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const unsigned int faction = GetFaction();
    const unsigned int sel = static_cast<unsigned int>(IsSelected());

    unsigned int texInd = ID_STRUCT_BASE_L1;

    if(NO_FACTION == faction)
        texInd = ID_STRUCT_BASE_L1 + sel;
    else
        texInd = ID_STRUCT_BASE_L1_F1 + (faction * NUM_BASE_SPRITES_PER_FAC) + sel;

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);

    isoObj->SetTexture(tex);
}

} // namespace game
