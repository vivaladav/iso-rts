#include "GameObjects/PracticeTarget.h"

#include "GameData.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

PracticeTarget::PracticeTarget(int rows, int cols)
    : GameObject(GameObjectType::OBJ_PRACTICE_TARGET, rows, cols)
{
    SetStructure(true);

    SetImage();
}

void PracticeTarget::UpdateImage()
{
    SetImage();
}

void PracticeTarget::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // set texture
    const unsigned int spriteId = SpriteIdSceneElements::SPR_PRACTICE_TARGET1;
    auto tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileSceneElements, spriteId);
    isoObj->SetTexture(tex);
}

} // namespace game
