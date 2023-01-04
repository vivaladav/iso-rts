#include "GameObjects/Trees.h"

#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/UniformDistribution.h>

namespace game
{

Trees::Trees(GameObjectType subtype, int rows, int cols)
    : GameObject(GameObjectType::OBJ_TREES, rows, cols)
    , mSubtype(subtype)
{
    SetStructure(true);

    // randomize initial variant
    sgl::utilities::UniformDistribution dis(0, NUM_TREE1_VARIANTS - 1);
    mVariant = dis.GetNextValue();

    //randomize time for change
    const int minTime = 2;
    const int maxTime = 6;
    dis.SetParameters(minTime, maxTime);

    mTimeChange = dis.GetNextValue();
    mTimerChange = mTimeChange;

    SetImage();
}

void Trees::Update(float delta)
{
    mTimerChange -= delta;

    const float zeroTime = 0.01f;

    if(mTimerChange < zeroTime)
    {
        if(mNumTrees < MAX_TREE1_TREES)
        {
            ++mNumTrees;

            SetImage();

            mTimerChange = mTimeChange;
        }
    }
}

void Trees::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void Trees::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    // set texture
    auto tm = sgl::graphic::TextureManager::Instance();

    sgl::graphic::Texture * tex = nullptr;

    const unsigned int baseSpriteId = TREE1_1T_1 + (NUM_TREE1_VARIANTS * (mNumTrees - 1));
    const unsigned int spriteId = baseSpriteId + mVariant;
    tex = tm->GetSprite(SpriteFileTrees, spriteId);

    isoObj->SetTexture(tex);
}

void Trees::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0x5d614bff);
    mObjColors.push_back(0x858a6bff);
    mObjColors.push_back(0x787d61ff);
    mObjColors.push_back(0x515441ff);
    mObjColors.push_back(0x6c7057ff);
    mObjColors.push_back(0x454838ff);
}

} // namespace game