#pragma once

#include <vector>

namespace lib { namespace graphic { class Image; } }

namespace game
{

class IsoMap
{
public:
    IsoMap(unsigned int rows, unsigned int cols);
    ~IsoMap();

    void AddTile(const char * file);

    void SetOrigin(int x, int y);

    void Render();

private:
    int mX0 = 0;
    int mY0 = 0;

    unsigned int mRows = 0;
    unsigned int mCols = 0;

    std::vector<unsigned int> mMap;
    std::vector<lib::graphic::Image *> mTiles;
};

inline void IsoMap::SetOrigin(int x, int y)
{
    mX0 = x;
    mY0 = y;
}

} // namespace game

