#include "IsoMap.h"

#include <graphic/Image.h>

#include <cassert>
#include <cmath>
#include <iostream>

const float COS_M45 =  0.70710678f;
const float SIN_M45 = -0.70710678f;

namespace game
{

IsoMap::IsoMap(unsigned int rows, unsigned int cols)
    : mRows(rows)
    , mCols(cols)
{
    const int size = rows * cols;

    mMap.reserve(size);

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

bool IsoMap::AddTile(const char * file)
{
    assert(file);

    using namespace lib::graphic;

    Image * img = new Image(file);

    const int imgW = img->GetWidth();
    const int imgH = img->GetHeight();

    // set size of tile when adding the first one
    if(mTiles.empty())
    {
        mTileW = imgW;
        mTileH = imgH;
        mTileHalfH = imgH * 0.5f;
    }
    else
    {
        if(mTileW != imgW || mTileH != imgH)
        {
            std::cerr << "IsoMap::AddTile - new tile size ("
                      << imgW << "," << imgH
                      << ") is differnt from allowed size ("
                      << mTileW << "," << mTileH << ")" << std::endl;

            delete img;

            return false;
        }
    }

    mTiles.emplace_back(img);

    return true;
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

            // x =  (h * c)   -   (h * r)   = h * (c - r)
            // y = (h/2 * c)  +  (h/2 * r)  = h/2 * (c + r)
            const int x = mRenderX0 + mTileH * (c - r);
            const int y = mY0 + mTileHalfH * (c + r);

            img->SetPosition(x, y);
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

} // namespace game
