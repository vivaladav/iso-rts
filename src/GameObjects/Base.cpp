#include "GameObjects/Base.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

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

    const Player * owner = GetOwner();

    // avoid to set an image when there's no owner set
    if(nullptr == owner)
        return ;

    const unsigned int faction = owner->GetFaction();

    const unsigned int texInd = SpriteIdStructures::ID_STRUCT_BASE_L1_F1 +
                                (faction * NUM_BASE_SPRITES_PER_FAC) +
                                static_cast<int>(IsSelected());

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);

    isoObj->SetTexture(tex);
}

} // namespace game
