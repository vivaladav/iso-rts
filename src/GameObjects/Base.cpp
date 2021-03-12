#include "GameObjects/Base.h"

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
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

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

    // object is selected
    if(IsSelected())
    {
        const char * imgFiles[] =
        {
            "data/img/base-f1-sel.png",
            "data/img/base-f2-sel.png",
            "data/img/base-f3-sel.png"
        };

        tex = tm->GetTexture(imgFiles[faction]);
    }
    // not selected
    else
    {
        const char * imgFiles[] =
        {
            "data/img/base-f1.png",
            "data/img/base-f2.png",
            "data/img/base-f3.png"
        };

        tex = tm->GetTexture(imgFiles[faction]);
    }

    isoObj->SetTexture(tex);
}

} // namespace game
