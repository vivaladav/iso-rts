#include "IsoMap.h"

#include <graphic/Image.h>

#include <cassert>
#include <iostream>

namespace game
{

IsoMap::IsoMap(unsigned int rows, unsigned int cols, int tileW)
    : mRows(rows)
    , mCols(cols)
    , mTileW(tileW)
    , mTileH(tileW * 0.5f)
    , mTileHalfH(tileW * 0.25f)
{
    const int size = rows * cols;

    mMap.reserve(size);
    mTilePositions.reserve(size);

    // TODO read map from file
    mMap.assign(size, 0);

    mMap[1] = 1;
    mMap[2 * mCols + 14] = 2;
}

IsoMap::~IsoMap()
{
    for(lib::graphic::Image * img : mTiles)
        delete img;
}

void IsoMap::SetTiles(const std::vector<std::string> & files)
{
    for(const std::string & file : files)
    {
        auto * img = new lib::graphic::Image(file.c_str());

        mTiles.emplace_back(img);
    }

    UpdateTilePositions();
}

void IsoMap::Render()
{
    for(unsigned int r = 0; r < mRows; ++r)
    {
        const unsigned int indb = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const unsigned int ind = indb + c;

            lib::graphic::Image * img = mTiles[mMap[ind]];

            const lib::core::Point2D & p = mTilePositions[ind];

            img->SetPosition(p.x, p.y);
            img->Render();
        }
    }
}

Cell2D IsoMap::TileFromScreenPoint(int x, int y) const
{
    const float xf = x - mX0;
    const float yf = y - mY0;
    const float w = mTileW;
    const float h = mTileH;

    const float col = (2. * yf + xf) / w;
    const float row = col - (xf / h);

    const Cell2D cell(static_cast<int>(row), static_cast<int>(col));
    return cell;
}

void IsoMap::UpdateTilePositions()
{
    for(unsigned int r = 0; r < mRows; ++r)
    {
        const unsigned int indb = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const unsigned int ind = indb + c;

            lib::core::Point2D & p = mTilePositions[ind];

            // x =  (h * c)   -   (h * r)   = h * (c - r)
            // y = (h/2 * c)  +  (h/2 * r)  = h/2 * (c + r)
            p.x = mRenderX0 + mTileH * (c - r);
            p.y = mY0 + mTileHalfH * (c + r);
        }
    }
}

} // namespace game
