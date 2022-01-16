#include "GameObjects/ObjectsDataRegistry.h"

#include "GameData.h"
#include "Structure.h"
#include "Unit.h"

namespace game
{

ObjectsDataRegistry::ObjectsDataRegistry()
{
    InitStructures();
    InitUnits();
}

const ObjectData & ObjectsDataRegistry::GetStructure(PlayerFaction f, StructureType type) const
{
    auto res = mStructures[f].find(type);

    if(res != mStructures[f].end())
        return res->second;
    else
        return ObjectData::NullObj;
}

const ObjectData & ObjectsDataRegistry::GetUnit(PlayerFaction f, UnitType type) const
{
    auto res = mUnits[f].find(type);

    if(res != mUnits[f].end())
        return res->second;
    else
        return ObjectData::NullObj;

}

void ObjectsDataRegistry::InitStructures()
{
    // -- FACTION 1 --
    std::unordered_map<StructureType, ObjectData> & data1 = mStructures[FACTION_1];

    // target
    data1.emplace
    (   STRUCT_TARGET,
        ObjectData(std::vector<int>{ 8, 2, 0, 0, 0, 5, 8, 8 },
                   std::vector<int>{ 50, 50, 0, 0 },
                   SpriteFileSceneElements,
                   SPR_PRACTICE_TARGET1,
                   OCS_TARGET,
                   STRUCT_TARGET)
    );

    // radar
    data1.emplace
    (
        STRUCT_RADAR,
        ObjectData(std::vector<int>{ 4, 8, 0, 0, 0, 4, 5, 3 },
                   std::vector<int>{ 100, 100, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_RADAR_F1,
                   OCS_RADAR,
                   STRUCT_RADAR)
    );

    // defensive tower
    data1.emplace
    (
        STRUCT_DEF_TOWER,
        ObjectData(std::vector<int>{ 5, 4, 2, 4, 4, 5, 5, 5 },
                   std::vector<int>{ 150, 150, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_DTOWER_L1_F1,
                   OCS_TOWER,
                   STRUCT_DEF_TOWER)
    );

    // -- FACTION 2 --
    std::unordered_map<StructureType, ObjectData> & data2 = mStructures[FACTION_2];

    // target
    data2.emplace
    (
        STRUCT_TARGET,
        ObjectData(std::vector<int>{ 8, 2, 0, 0, 0, 5, 8, 8 },
                   std::vector<int>{ 50, 50, 0, 0 },
                   SpriteFileSceneElements,
                   SPR_PRACTICE_TARGET1,
                   OCS_TARGET,
                   STRUCT_TARGET)
    );

    // radar
    data2.emplace
    (
        STRUCT_RADAR,
        ObjectData(std::vector<int>{ 4, 8, 0, 0, 0, 4, 5, 3 },
                   std::vector<int>{ 100, 100, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_RADAR_F2,
                   OCS_RADAR,
                   STRUCT_RADAR)
    );

    // defensive tower
    data2.emplace
    (
        STRUCT_DEF_TOWER,
        ObjectData(std::vector<int>{ 5, 4, 2, 4, 4, 5, 5, 5 },
                   std::vector<int>{ 150, 150, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_DTOWER_L1_F2,
                   OCS_TOWER,
                   STRUCT_DEF_TOWER)
    );

    // -- FACTION 3 --
    std::unordered_map<StructureType, ObjectData> & data3 = mStructures[FACTION_3];

    // target
    data3.emplace
    (
        STRUCT_TARGET,
        ObjectData(std::vector<int>{ 8, 2, 0, 0, 0, 5, 8, 8 },
                   std::vector<int>{ 50, 50, 0, 0 },
                   SpriteFileSceneElements,
                   SPR_PRACTICE_TARGET1,
                   OCS_TARGET,
                   STRUCT_TARGET)
    );

    // radar
    data3.emplace
    (
        STRUCT_RADAR,
        ObjectData(std::vector<int>{ 4, 8, 0, 0, 0, 4, 5, 3 },
                   std::vector<int>{ 100, 100, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_RADAR_F3,
                   OCS_RADAR,
                   STRUCT_RADAR)
    );

    // defensive tower
    data3.emplace
    (
        STRUCT_DEF_TOWER,
        ObjectData(std::vector<int>{ 5, 4, 2, 4, 4, 5, 5, 5 },
                   std::vector<int>{ 150, 150, 0, 0 },
                   SpriteFileStructures,
                   ID_STRUCT_DTOWER_L1_F3,
                   OCS_TOWER,
                   STRUCT_DEF_TOWER)
    );
}

void ObjectsDataRegistry::InitUnits()
{
    // -- FACTION 1 --
    std::unordered_map<UnitType, ObjectData> & data1 = mUnits[FACTION_1];

    // unit 1
    data1.emplace
    (
        UNIT_1,
        ObjectData(std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
                   std::vector<int>{ 15, 15, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_01_F1,
                   OCU_WORKER,
                   UNIT_1)
    );

    // unit 2
    data1.emplace
    (
        UNIT_2,
        ObjectData(std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
                   std::vector<int>{ 20, 20, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_02_F1,
                   OCU_SOLDIER,
                   UNIT_2)
    );

    // unit 3
    data1.emplace
    (
        UNIT_3,
        ObjectData(std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
                   std::vector<int>{ 40, 40, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_03_F1,
                   OCU_GENERIC,
                   UNIT_3)

    );

    // -- FACTION 2 --
    std::unordered_map<UnitType, ObjectData> & data2 = mUnits[FACTION_2];

    // unit 1
    data2.emplace
    (
        UNIT_1,
        ObjectData(std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
                   std::vector<int>{ 15, 15, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_01_F2,
                   OCU_WORKER,
                   UNIT_1)
    );

    // unit 2
    data2.emplace
    (
        UNIT_2,
        ObjectData(std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
                   std::vector<int>{ 20, 20, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_02_F2,
                   OCU_SOLDIER,
                   UNIT_2)
    );

    // unit 3
    data2.emplace
    (
        UNIT_3,
        ObjectData(std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
                   std::vector<int>{ 40, 40, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_03_F2,
                   OCU_GENERIC,
                   UNIT_3)
    );

    // -- FACTION 3 --
    std::unordered_map<UnitType, ObjectData> & data3 = mUnits[FACTION_3];

    // unit 1
    data3.emplace
    (
        UNIT_1,
        ObjectData(std::vector<int>{ 4, 4, 1, 1, 1, 4, 2, 2, 5, 6, 6 },
                   std::vector<int>{ 15, 15, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_01_F3,
                   OCU_WORKER,
                   UNIT_1)
    );

    // unit 2
    data3.emplace
    (
        UNIT_2,
        ObjectData(std::vector<int>{ 4, 5, 5, 4, 5, 3, 3, 2, 6, 1, 4 },
                   std::vector<int>{ 20, 20, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_02_F3,
                   OCU_SOLDIER,
                   UNIT_2)
    );

    // unit 3
    data3.emplace
    (
        UNIT_3,
        ObjectData(std::vector<int>{ 6, 5, 2, 5, 3, 4, 5, 5, 3, 3, 4 },
                   std::vector<int>{ 40, 40, 0, 0 },
                   SpriteFileUnits,
                   SPR_UNIT_03_F3,
                   OCU_GENERIC,
                   UNIT_3)
    );
}

} // namespace game
