#include "ResourceGenerator.h"

#include "GameObjectEnums.h"

namespace game
{

ResourceGenerator::ResourceGenerator(ResourceType type)
    : GameObject(OBJ_RES_GEN, -1)
    , mResType(type)
{
    SetImageId();

    UpdateOutput();
}

void ResourceGenerator::UpdateImageId()
{
    SetImageId();
}

void ResourceGenerator::SetImageId()
{
    const int owner = GetOwner();

    if(ENERGY == mResType)
    {
        if(owner != -1)
            mImageId = static_cast<GameObjectImageId>(GameObjectImageId::ENERGY_SOURCE_F1 + owner);
        else
            mImageId = GameObjectImageId::ENERGY_SOURCE;
    }
    else if(MATERIAL1 == mResType)
    {
        if(owner != -1)
            mImageId = static_cast<GameObjectImageId>(GameObjectImageId::MATERIAL1_SOURCE_F1 + owner);
        else
            mImageId = GameObjectImageId::MATERIAL1_SOURCE;
    }
    else
        mImageId = IMG_NULL;
}

void ResourceGenerator::UpdateOutput()
{
    if(ENERGY == mResType)
        mOutput = 20;
    else if(MATERIAL1 == mResType)
        mOutput = 2;
    // default
    else
        mOutput = 1;
}

} // namespace game

