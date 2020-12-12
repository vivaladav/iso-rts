#include "ResourceGenerator.h"

#include "GameObjectEnums.h"

namespace game
{

ResourceGenerator::ResourceGenerator(int owner, ResourceType)
    : GameObject(OBJ_RES_GEN, owner)
{
    SetImageId();
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

} // namespace game

