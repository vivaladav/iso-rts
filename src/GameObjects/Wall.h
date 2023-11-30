#pragma once

#include "Structure.h"

namespace game
{

class Wall : public Structure
{
public:
    enum WallPart : GameObjectVariantId
    {
        HORIZ,
        VERT,
        TL,
        TR,
        BL,
        BR,
        INTN,
        INTS,
        INTW,
        INTE,
        CROSS,

        NUM_WALL_PARTS
    };

public:
    Wall(GameObjectVariantId part);

    static unsigned int GetCostEnergy(unsigned int level);
    static unsigned int GetCostMaterial(unsigned int level);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();
};

} // namespace game
