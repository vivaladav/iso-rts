#include "GameObjects/Wall.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

Wall::Wall(GameObjectVariantId part, int rows, int cols)
    : Structure(GameObject::TYPE_WALL, GameObject::CAT_GENERIC, rows, cols)
    , mPart(part)
{
    SetImage();
}

void Wall::SetWallType(GameObjectVariantId part)
{
    mPart = part;

    UpdateGraphics();
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

void Wall::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void Wall::SetImage()
{
    auto * tm = sgl::graphic::TextureManager::Instance();

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

    const int ind = SpriteWallsId::WALL_L1_F1_HORIZ + mPart + (faction * NUM_WALL_PARTS);

    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileWalls, ind);
    isoObj->SetTexture(tex);
}

} // namespace game
