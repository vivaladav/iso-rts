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
        std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F1,
        OCU_GENERIC
    );

    // unit 2
    data1.emplace_back
    (
        std::vector<int>{ 1, 4, 3, 2, 5, 6, 1, 8, 9, 9, 9 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F1,
        OCU_GENERIC
    );

    // unit 3
    data1.emplace_back
    (
        std::vector<int>{ 1, 3, 3, 4, 5, 1, 1, 8, 9, 2, 7 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F1,
        OCU_GENERIC
    );

    // -- FACTION 2 --
    std::vector<ObjectData> & data2 = mUnits[FACTION_2];

    // unit 1
    data2.emplace_back
    (
        std::vector<int>{ 1, 2, 3, 4, 1, 6, 7, 8, 1, 2, 3 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F2,
        OCU_GENERIC
    );

    // unit 2
    data2.emplace_back
    (
        std::vector<int>{ 1, 2, 9, 4, 5, 2, 7, 8, 9, 1, 2 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F2,
        OCU_GENERIC
    );

    // unit 3
    data2.emplace_back
    (
        std::vector<int>{ 1, 9, 3, 4, 2, 6, 7, 8, 1, 3, 5 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F2,
        OCU_GENERIC
    );

    // -- FACTION 3 --
    std::vector<ObjectData> & data3 = mUnits[FACTION_3];

    // unit 1
    data3.emplace_back
    (
        std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_01_F3,
        OCU_GENERIC
    );

    // unit 2
    data3.emplace_back
    (
        std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_02_F3,
        OCU_GENERIC
    );

    // unit 3
    data3.emplace_back
    (
        std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9 },
        std::vector<int>{ 10, 10, 0, 0 },
        SpriteFileUnits,
        SPR_UNIT_03_F3,
        OCU_GENERIC
    );
}

} // namespace game
