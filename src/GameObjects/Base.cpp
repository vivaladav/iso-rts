#include "GameObjects/Base.h"

#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

Base::Base(int owner, int rows, int cols)
    : GameObject(GameObjectType::OBJ_BASE, owner, rows, cols)
{
    SetImage();
}

void Base::UpdateImage()
{
    SetImage();
}

void Base::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

    const int owner = GetOwner();

    // object is selected
    if(IsSelected())
    {
        const char * imgFiles[] =
        {
            "data/img/base-f1-sel.png",
            "data/img/base-f2-sel.png",
            "data/img/base-f3-sel.png"
        };

        tex = tm->GetTexture(imgFiles[owner]);
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

        tex = tm->GetTexture(imgFiles[owner]);
    }

    mIsoObj->SetTexture(tex);
}

} // namespace game
