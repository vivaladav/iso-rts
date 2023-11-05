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
    if(objTop != nullptr && objTop->GetObjectCategory() == GameObject::CAT_UNIT)
        return static_cast<Unit *>(objTop);
    else
        return nullptr;
}

bool GameMapCell::HasUnit() const
{
    return objTop != nullptr && objTop->GetObjectCategory() == GameObject::CAT_UNIT;
}

ResourceGenerator * GameMapCell::GetResourceGenerator() const
{
    if(objTop != nullptr && objTop->GetObjectCategory() == GameObject::CAT_RES_GENERATOR)
        return static_cast<ResourceGenerator *>(objTop);
    else
        return nullptr;
}

} // namespace game
