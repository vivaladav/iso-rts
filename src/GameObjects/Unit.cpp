#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

Unit::Unit(int rows, int cols)
    : GameObject(GameObjectType::OBJ_UNIT, rows, cols)
{
    SetSpeed(2.f);
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
            "data/img/unit_01-f1-sel.png",
            "data/img/unit_01-f2-sel.png",
            "data/img/unit_01-f3-sel.png"
        };

        tex = tm->GetTexture(imgFiles[faction]);
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

        tex = tm->GetTexture(imgFiles[faction]);
    }

    GetIsoObject()->SetTexture(tex);
}

} // namespace game
