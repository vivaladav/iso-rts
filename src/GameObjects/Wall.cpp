#include "GameObjects/Wall.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

Wall::Wall(GameObjectType subtype, int rows, int cols)
    : GameObject(GameObjectType::OBJ_WALL, rows, cols)
    , mSubtypeInd(subtype - GameObjectType::OBJ_WALL_FIRST)
{
    SetStructure(true);

    SetImage();
}

void Wall::SetWallType(GameObjectType type)
{
    mSubtypeInd = type - GameObjectType::OBJ_WALL_FIRST;

    UpdateImage();
}

unsigned int Wall::GetCostEnergy(unsigned int level)
{
    const unsigned int cost0 = 5;
    return (level + 1) * cost0;
}

unsigned int Wall::GetCostMaterial(unsigned int level)
{
    const unsigned int cost0 = 10;
    return (level + 1) * cost0;
}

void Wall::UpdateImage()
{
    SetImage();
}

void Wall::SetImage()
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

    const int ind = SpriteWallsId::WALL_L1_F1_HORIZ + mSubtypeInd + (faction * NUM_OBJS_WALL);

    lib::graphic::Texture * tex = tm->GetSprite(SpriteWallsFile, ind);
    isoObj->SetTexture(tex);
}

} // namespace game
