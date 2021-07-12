#include "GameObjects/SceneObject.h"

#include "GameData.h"
#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

SceneObject::SceneObject(GameObjectType subtype, int rows, int cols)
    : GameObject(GameObjectType::OBJ_SCENE_OBJECT, rows, cols)
    , mSubtypeInd(subtype - GameObjectType::OBJ_ROCKS_FIRST)
{
    SetStructure(true);

    SetImage();
}

void SceneObject::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void SceneObject::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // set texture
    const unsigned int spriteId = SpriteRocksId::ROCKS_ROW_END_L_1 + mSubtypeInd;
    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = tm->GetSprite(SpriteRocksFile, spriteId);
    isoObj->SetTexture(tex);
}

void SceneObject::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xad9485ff);
    mObjColors.push_back(0xa38776ff);
    mObjColors.push_back(0x997966ff);
    mObjColors.push_back(0x7a6152ff);
    mObjColors.push_back(0x6b5548ff);
    mObjColors.push_back(0x584a41ff);
}

} // namespace game
