#include "GameObjects/ObjectsDataRegistry.h"

#include "GameData.h"

namespace game
{

ObjectsDataRegistry::ObjectsDataRegistry()
{
    InitStructures();
    InitUnits();
}

void ObjectsDataRegistry::InitStructures()
{

}

void ObjectsDataRegistry::InitUnits()
{
    // -- FACTION 1--
    std::vector<ObjectData> & data1 = mUnits[FACTION_1];

    // unit 1
    data1.emplace_back
    (
        std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
        std::vector<int>{ 15, 15, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F1,
        OCU_WORKER
    );

    // unit 2
    data1.emplace_back
    (
        std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
        std::vector<int>{ 20, 20, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F1,
        OCU_SOLDIER
    );

    // unit 3
    data1.emplace_back
    (
        std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
        std::vector<int>{ 40, 40, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F1,
        OCU_GENERIC
    );

    // -- FACTION 2 --
    std::vector<ObjectData> & data2 = mUnits[FACTION_2];

    // unit 1
    data2.emplace_back
    (
        std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
        std::vector<int>{ 15, 15, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F2,
        OCU_WORKER
    );

    // unit 2
    data2.emplace_back
    (
        std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
        std::vector<int>{ 20, 20, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F2,
        OCU_SOLDIER
    );

    // unit 3
    data2.emplace_back
    (
        std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
        std::vector<int>{ 40, 40, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F2,
        OCU_GENERIC
    );

    // -- FACTION 3 --
    std::vector<ObjectData> & data3 = mUnits[FACTION_3];

    // unit 1
    data3.emplace_back
    (
        std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
        std::vector<int>{ 15, 15, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F3,
        OCU_WORKER
    );

    // unit 2
    data3.emplace_back
    (
        std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
        std::vector<int>{ 20, 20, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F3,
        OCU_SOLDIER
    );

    // unit 3
    data3.emplace_back
    (
        std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
        std::vector<int>{ 40, 40, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F3,
        OCU_GENERIC
    );
}

} // namespace game
