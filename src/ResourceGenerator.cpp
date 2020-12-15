#include "ResourceGenerator.h"

#include "GameObjectEnums.h"

namespace game
{

ResourceGenerator::ResourceGenerator(int owner, ResourceType type)
    : GameObject(OBJ_RES_GEN, owner)
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

    if(owner != -1)
        mImageId = static_cast<GameObjectImageId>(GameObjectImageId::ENERGY_SOURCE_P1 + owner);
    else
        mImageId = GameObjectImageId::ENERGY_SOURCE;
}

void ResourceGenerator::UpdateOutput()
{
    if(ENERGY == mResType)
    {
        mOutput = 20;
    }
}

} // namespace game

