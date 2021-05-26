#include "GameObjects/Base.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

Base::Base(int rows, int cols)
    : GameObject(GameObjectType::OBJ_BASE, rows, cols)
{
    SetStructure(true);

    SetVisibilityLevel(1);
}

void Base::UpdateImage()
{
    SetImage();
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

    auto * tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);

    isoObj->SetTexture(tex);
}

} // namespace game
