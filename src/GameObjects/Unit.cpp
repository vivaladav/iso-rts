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
        // TODO add selected images
        const char * imgFiles[] =
        {
            "data/img/unit_01-f1.png",
            "data/img/unit_01-f2.png",
            "data/img/unit_01-f3.png"
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

    mIsoObj->SetTexture(tex);
}

} // namespace game
