#include "GameObjects/SceneObject.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

SceneObject::SceneObject(GameObjectTypeId type, GameObjectVariantId part, int rows, int cols)
    : GameObject(type, CAT_SCENE_OBJ, rows, cols)
{
    SetStatic(true);

    mVariant = part;

    // set object health
    float health = 1000.f;

    if(TYPE_MOUNTAINS == type)
        const float health = 10000.f;

    SetMaxHealth(health);
    SetHealth(health);

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

    const GameObjectTypeId type = GetObjectType();

    if(type == TYPE_ROCKS)
    {
        const unsigned int spriteId = SpriteRocksId::ROCKS_ROW_END_L_1 + mVariant;
        tex = tm->GetSprite(SpriteRocksFile, spriteId);
    }
    else if(type == TYPE_MOUNTAINS)
    {
        const int sel = static_cast<int>(IsSelected());
        const unsigned int spriteId = ID_SCENE_MOUNTAIN_L + mVariant + (sel * NUM_MOUNTAINS_SPRITES);
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
