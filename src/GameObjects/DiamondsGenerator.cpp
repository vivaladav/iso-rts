#include "GameObjects/DiamondsGenerator.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "GameObjects/Diamonds.h"

#include <utilities/UniformDistribution.h>

namespace game
{

DiamondsGenerator::DiamondsGenerator(GameMap * gm)
    : CollectableGenerator(gm, 10, 60)
{
}

void DiamondsGenerator::OnGeneration()
{
    GameMap * gm = GetGameMap();

    const int r = GetRow();
    const int c = GetCol();

    GameObject * obj = gm->CreateObject(MapLayers::OBJECTS, GameObjectType::OBJ_DIAMONDS,
                                        nullptr, r, c, 1, 1);

    if(nullptr == obj)
        return ;

    auto diamonds = static_cast<Diamonds *>(obj);

    // set number of diamonds in cell
    lib::utilities::UniformDistribution ran(Diamonds::MIN_UNITS, Diamonds::MAX_UNITS);
    const int num = ran.GetNextValue();
    diamonds->SetNum(num);

    diamonds->SetGenerator(this);

    // update visibility
    gm->ApplyLocalVisibilityToObject(obj);

    // make the cell walkable for collection
    gm->SetCellWalkable(r, c, true);
}

} // namespace game
