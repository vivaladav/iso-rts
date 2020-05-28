#pragma once

#include <core/Point.h>

#include <string>
#include <vector>

namespace lib { namespace graphic { class Image; } }

namespace game
{

struct Cell2D
{
    Cell2D(int r, int c) : row(r), col(c) {}

    int row = 0;
    int col = 0;
};

class IsoMap
{
public:
    IsoMap(unsigned int rows, unsigned int cols, int tileW);
    ~IsoMap();

    void SetTiles(const std::vector<std::string> & files);

    int GetTileWidth() const;
    int GetTileHeight() const;

    int GetWidth() const;
    int GetHeight() const;

    void SetOrigin(int x, int y);

    bool Load(const char * file);

    void Render();

    Cell2D TileFromScreenPoint(int x, int y) const;

private:
    void UpdateTilePositions();

private:
    unsigned int mRows = 0;
    unsigned int mCols = 0;

    int mX0 = 0;
    int mRenderX0 = 0;
    int mY0 = 0;

    int mTileW = 0;
    int mTileH = 0;
    int mTileHalfH = 0;
    int mOrthoTileSize = 0;

    std::vector<unsigned int> mMap;
    std::vector<lib::graphic::Image *> mTiles;
    std::vector<lib::core::Point2D> mTilePositions;
};

inline int IsoMap::GetTileWidth() const { return mTileW; }
inline int IsoMap::GetTileHeight() const { return mTileH; }

inline int IsoMap::GetWidth() const { return mTileW * mCols; }
inline int IsoMap::GetHeight() const { return mTileH * mRows; }

inline void IsoMap::SetOrigin(int x, int y)
{
    mX0 = x;
    mY0 = y;

    // rendering x is shifted of -w/2 because image origin is TL corner
    mRenderX0 = x - mTileH;

    UpdateTilePositions();
}

} // namespace game

