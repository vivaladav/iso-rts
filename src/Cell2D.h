#pragma once

namespace game
{

struct Cell2D
{
    Cell2D(int r, int c) : row(r), col(c) {}

    int row = 0;
    int col = 0;

    friend bool operator==(const Cell2D & c1, const Cell2D & c2);
};

inline bool operator==(const Cell2D & c1, const Cell2D & c2)
{
    return c1.row == c2.row && c1.col == c2.col;
}

} // namespace game
