#pragma once

namespace game
{

struct Cell2D
{
    Cell2D(int r, int c) : row(r), col(c) {}

    int row = 0;
    int col = 0;
};

} // namespace game
