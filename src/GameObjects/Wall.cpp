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

    const char * imgFiles[] =
    {
        "data/img/wall_v-f1.png",
        "data/img/wall_v-f1.png",
        "data/img/wall_v-f1.png",
        "data/img/wall_v-f1.png",
        "data/img/wall_v-f1.png",
        "data/img/wall_v-f1.png",

        "data/img/wall_v-f2.png",
        "data/img/wall_v-f2.png",
        "data/img/wall_v-f2.png",
        "data/img/wall_v-f2.png",
        "data/img/wall_v-f2.png",
        "data/img/wall_v-f2.png",

        "data/img/wall_v-f3.png",
        "data/img/wall_v-f3.png",
        "data/img/wall_v-f3.png",
        "data/img/wall_v-f3.png",
        "data/img/wall_v-f3.png",
        "data/img/wall_v-f3.png"
    };

    const int ind = mSubtypeInd + (faction * NUM_OBJS_WALL);

    lib::graphic::Texture * tex = tm->GetTexture(imgFiles[ind]);
    isoObj->SetTexture(tex);
}

} // namespace game
