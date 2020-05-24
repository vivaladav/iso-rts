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

    bool AddTile(const char * file);

    int GetTileWidth() const;
    int GetTileHeight() const;

    int GetWidth() const;
    int GetHeight() const;

    void SetOrigin(int x, int y);

    void Render();

private:
    int mX0 = 0;
    int mY0 = 0;

    int mTileW = 0;
    int mTileH = 0;
    int mTileHalfH = 0;

    unsigned int mRows = 0;
    unsigned int mCols = 0;

    std::vector<unsigned int> mMap;
    std::vector<lib::graphic::Image *> mTiles;
};

inline int IsoMap::GetTileWidth() const { return mTileW; }
inline int IsoMap::GetTileHeight() const { return mTileH; }

inline int IsoMap::GetWidth() const { return mTileW * mCols; }
inline int IsoMap::GetHeight() const { return mTileH * mRows; }

inline void IsoMap::SetOrigin(int x, int y)
{
    mX0 = x;
    mY0 = y;
}

} // namespace game

