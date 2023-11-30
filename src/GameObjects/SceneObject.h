#pragma once

#include "GameObject.h"

namespace game
{

class SceneObject : public GameObject
{
public:
    enum SceneObjectParts : GameObjectVariantId
    {
        // MOUNTAINS
        MOUNTAINS_L = 0,
        MOUNTAINS_T,
        MOUNTAINS_B,
        MOUNTAINS_R,
        MOUNTAINS_1,
        MOUNTAINS_2,
        MOUNTAINS_3,
        MOUNTAINS_4,
        MOUNTAINS_5,
        MOUNTAINS_6,
        MOUNTAINS_7,
        MOUNTAINS_8,

        NUM_MOUNTAINS_PARTS,

        // ROCKS
        ROCKS_ROW_END_L_1 = 0,
        ROCKS_ROW_END_R_1,
        ROCKS_COL_END_L_1,
        ROCKS_COL_END_R_1,
        ROCKS_ROW_1,
        ROCKS_ROW_2,
        ROCKS_ROW_3,
        ROCKS_COL_1,
        ROCKS_COL_2,
        ROCKS_COL_3,
        ROCKS_CORN_BL_1,
        ROCKS_CORN_BR_1,
        ROCKS_CORN_TL_1,
        ROCKS_CORN_TR_1,
        ROCKS_INTN,
        ROCKS_INTS,
        ROCKS_INTW,
        ROCKS_INTE,
        ROCKS_CROSS,

        NUM_ROCKS_PARTS
    };

public:
    SceneObject(GameObjectTypeId type, GameObjectVariantId part, int rows, int cols);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();
};

} // namespace game
