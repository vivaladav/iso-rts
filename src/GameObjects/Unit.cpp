#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

Unit::Unit(int owner, int rows, int cols)
    : GameObject(GameObjectType::OBJ_UNIT, owner, rows, cols)
{
    SetImage();

    SetSpeed(10.f);
}

void Unit::IncreaseUnitLevel()
{
    if(mLevel >= MAX_UNITS_LEVEL)
        return ;

    ++mLevel;
    SetImage();
}

void Unit::UpdateImage()
{
    SetImage();
}

void Unit::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

    const int owner = GetOwner();

    // object is selected
    if(IsSelected())
    {
        const char * imgFiles[] =
        {
            "data/img/unit_01-f1-sel.png",
            "data/img/unit_01-f2-sel.png",
            "data/img/unit_01-f3-sel.png"
        };

        tex = tm->GetTexture(imgFiles[owner]);
    }
    // not selected
    else
    {
        const char * imgFiles[] =
        {
            "data/img/unit_01-f1.png",
            "data/img/unit_01-f2.png",
            "data/img/unit_01-f3.png"
        };

        tex = tm->GetTexture(imgFiles[owner]);
    }

    GetIsoObject()->SetTexture(tex);
}

} // namespace game
