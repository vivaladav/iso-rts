#include "GameObjects/BlobsGenerator.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "GameObjects/Blobs.h"

#include <sgl/utilities/UniformDistribution.h>

namespace game
{

BlobsGenerator::BlobsGenerator(GameMap * gm)
    : CollectableGenerator(gm, 10, 40)
{
}

void BlobsGenerator::OnGeneration()
{
    GameMap * gm = GetGameMap();

    const int r = GetRow();
    const int c = GetCol();

    GameObject * obj = gm->CreateObject(MapLayers::OBJECTS2, GameObjectType::OBJ_BLOBS,
                                        nullptr, r, c, 1, 1);

    if(nullptr == obj)
        return ;

    auto blobs = static_cast<Blobs *>(obj);

    // set number of Blobs in cell
    sgl::utilities::UniformDistribution ran(Blobs::MIN_UNITS, Blobs::MAX_UNITS);
    const int num = ran.GetNextValue();
    blobs->SetNum(num);

    blobs->SetGenerator(this);

    // update visibility
    gm->ApplyLocalVisibilityToObject(obj);

    // make the cell walkable for collection
    gm->SetCellWalkable(r, c, true);
}

} // namespace game
