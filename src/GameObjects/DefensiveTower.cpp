#include "GameObjects/DefensiveTower.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

DefensiveTower::DefensiveTower(int rows, int cols)
    : GameObject(GameObjectType::OBJ_DEF_TOWER, rows, cols)
{
    SetStructure(true);

    SetImage();
}

void DefensiveTower::UpdateImage()
{
    SetImage();
}

void DefensiveTower::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const Player * owner = GetOwner();

    // avoid to set an image when there's no owner set
    if(nullptr == owner)
        return ;

    // set texture
    const unsigned int faction = owner->GetFaction();

    const int ind = SpriteIdStructures::ID_STRUCT_TOWER_L1_F1 + faction;

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, ind);
    isoObj->SetTexture(tex);
}

} // namespace game
