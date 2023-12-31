#include "GameMapCell.h"

#include "GameConstants.h"
#include "GameObjects/ResourceGenerator.h"
#include "GameObjects/Unit.h"

namespace game
{

const std::unordered_set<CellTypes> GameMapCell::PRIMARY_TYPES =
{
    SCENE_ROCKS,
    NO_FACTION_OBJ,
    BLOBS_SOURCE,
    DIAMONDS_SOURCE,
    TREES1,
    F1,
    F1_CONNECTED,
    F2,
    F2_CONNECTED,
    F3,
    F3_CONNECTED,
};

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
