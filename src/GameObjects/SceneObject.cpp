#include "GameObjects/SceneObject.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

SceneObject::SceneObject(GameObjectType subtype, int rows, int cols)
    : GameObject(GameObjectType::OBJ_SCENE_OBJECT, rows, cols)
    , mSubtype(subtype)
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
    auto tm = sgl::graphic::TextureManager::Instance();

    sgl::graphic::Texture * tex = nullptr;

    if(mSubtype >= OBJ_ROCKS_FIRST && mSubtype <= OBJ_ROCKS_LAST)
    {
        const unsigned int ind = (mSubtype - OBJ_ROCKS_FIRST);
        const unsigned int spriteId = SpriteRocksId::ROCKS_ROW_END_L_1 + ind;
        tex = tm->GetSprite(SpriteRocksFile, spriteId);
    }
    else if(mSubtype >= OBJ_MOUNTAINS_FIRST && mSubtype <= OBJ_MOUNTAINS_LAST)
    {
        const unsigned int ind = (mSubtype - OBJ_MOUNTAINS_FIRST);
        const unsigned int spriteId = SpriteIdSceneElements::ID_SCENE_MOUNTAIN_L + ind;
        tex = tm->GetSprite(SpriteFileSceneElements, spriteId);
    }
    // this should never happen
    else
        return ;

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
