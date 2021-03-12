#include "GameObjects/SceneObject.h"

#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

SceneObject::SceneObject(GameObjectType subtype, int rows, int cols)
    : GameObject(GameObjectType::OBJ_SCENE_OBJECT, rows, cols)
    , mSubtypeInd(subtype - GameObjectType::OBJ_MOUNTAIN_FIRST)
{
    SetStructure(true);

    SetImage();
}

void SceneObject::UpdateImage()
{
    SetImage();
}

void SceneObject::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const char * imgFiles[] =
    {
        "data/img/mountain_row_end_l_1.png",
        "data/img/mountain_row_end_r_1.png",
        "data/img/mountain_col_end_l_1.png",
        "data/img/mountain_col_end_r_1.png",
        "data/img/mountain_row_1.png",
        "data/img/mountain_row_2.png",
        "data/img/mountain_row_3.png",
        "data/img/mountain_col_1.png",
        "data/img/mountain_col_2.png",
        "data/img/mountain_col_3.png",
        "data/img/mountain_corn_bl_1.png",
        "data/img/mountain_corn_br_1.png",
        "data/img/mountain_corn_tl_1.png",
        "data/img/mountain_corn_tr_1.png",
    };

    tex = tm->GetTexture(imgFiles[mSubtypeInd]);

    isoObj->SetTexture(tex);
}

} // namespace game
