#include "GameMapCell.h"

#include "GameConstants.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/Unit.h"

namespace game
{

GameMapCell::GameMapCell()
    : influencer(NO_FACTION)
{
    // init influencers map
    influencers[0] = false;
    influencers[1] = false;
    influencers[2] = false;
}

Unit * GameMapCell::GetUnit() const
{
    if(objTop != nullptr && objTop->GetObjectType() == GameObject::TYPE_UNIT)
        return static_cast<Unit *>(objTop);
    else
        return nullptr;
}

bool GameMapCell::HasUnit() const
{
    return objTop != nullptr && objTop->GetObjectType() == GameObject::TYPE_UNIT;
}

ResourceGenerator * GameMapCell::GetResourceGenerator() const
{
    if(objTop != nullptr &&
      (objTop->GetObjectType() == GameObject::TYPE_RES_GEN_ENERGY ||
       objTop->GetObjectType() == GameObject::TYPE_RES_GEN_ENERGY_SOLAR ||
       objTop->GetObjectType() == GameObject::TYPE_RES_GEN_MATERIAL ||
       objTop->GetObjectType() == GameObject::TYPE_RES_GEN_MATERIAL_EXTRACT))
        return static_cast<ResourceGenerator *>(objTop);
    else
        return nullptr;
}

} // namespace game
