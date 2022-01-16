#pragma once

#include "Structure.h"

namespace game
{

class Wall : public Structure
{
public:
    Wall(GameObjectType subtype, int rows, int cols);

    void SetWallType(GameObjectType type);

    static unsigned int GetCostEnergy(unsigned int level);
    static unsigned int GetCostMaterial(unsigned int level);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

private:
    int mSubtypeInd = 0;
};

} // namespace game
