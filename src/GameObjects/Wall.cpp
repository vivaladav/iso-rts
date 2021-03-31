#include "GameObjects/Wall.h"

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

    const unsigned int faction = owner->GetFaction();

    static const char * imgFiles[] =
    {
        "data/img/wall_h-f1.png",
        "data/img/wall_v-f1.png",
        "data/img/wall_tl-f1.png",
        "data/img/wall_tr-f1.png",
        "data/img/wall_bl-f1.png",
        "data/img/wall_br-f1.png",
        "data/img/wall_intn-f1.png",
        "data/img/wall_ints-f1.png",
        "data/img/wall_intw-f1.png",
        "data/img/wall_inte-f1.png",
        "data/img/wall_cross-f1.png",

        "data/img/wall_h-f2.png",
        "data/img/wall_v-f2.png",
        "data/img/wall_tl-f2.png",
        "data/img/wall_tr-f2.png",
        "data/img/wall_bl-f2.png",
        "data/img/wall_br-f2.png",
        "data/img/wall_intn-f2.png",
        "data/img/wall_ints-f2.png",
        "data/img/wall_intw-f2.png",
        "data/img/wall_inte-f2.png",
        "data/img/wall_cross-f2.png",

        "data/img/wall_h-f3.png",
        "data/img/wall_v-f3.png",
        "data/img/wall_tl-f3.png",
        "data/img/wall_tr-f3.png",
        "data/img/wall_bl-f3.png",
        "data/img/wall_br-f3.png",
        "data/img/wall_intn-f3.png",
        "data/img/wall_ints-f3.png",
        "data/img/wall_intw-f3.png",
        "data/img/wall_inte-f3.png",
        "data/img/wall_cross-f3.png"
    };

    const int ind = mSubtypeInd + (faction * NUM_OBJS_WALL);

    lib::graphic::Texture * tex = tm->GetTexture(imgFiles[ind]);
    isoObj->SetTexture(tex);
}

} // namespace game
