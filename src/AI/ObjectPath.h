#pragma once

#include "Cell2D.h"

#include <vector>

namespace game
{

class ObjectPath
{
public:
    ObjectPath();

private:
    std::vector<Cell2D> mCells;
};

} // namespace game
