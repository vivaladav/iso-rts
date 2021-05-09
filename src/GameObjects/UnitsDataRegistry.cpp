#include "GameObjects/UnitsDataRegistry.h"

#include "GameObjects/Unit.h"

namespace game
{

UnitsDataRegistry::UnitsDataRegistry()
{
    // UNIT 1
    mData.emplace_back
    (
        // type
        UNIT_1,
        // stats : energy, resistance, attack, speed, construction, conquest
        3, 2, 4, 4, 2, 2,
        // costs : energy, material
        10, 20
    );

    // UNIT 2
    mData.emplace_back
    (
        UNIT_2,
        3, 3, 1, 3, 4, 3,
        15, 25
    );

    // UNIT 3
    mData.emplace_back
    (
        UNIT_3,
        3, 4, 3, 2, 1, 1,
        30, 40
    );
}

} // namespace game
