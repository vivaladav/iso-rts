#include "IsoMap.h"

#include <graphic/Image.h>

#include <cassert>
#include <iostream>

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
}

IsoMap::~IsoMap()
{
    for(lib::graphic::Image * img : mTiles)
        delete img;
}

void IsoMap::AddTile(const char * file)
{
    assert(file);

    mTiles.emplace_back(new lib::graphic::Image(file));
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
            const int imgW = img->GetWidth();
            const int imgHalfW = imgW * 0.5f;
            const int imgH = img->GetHeight();
            const int imgHalfH = imgH * 0.5f;

            // x = h * c - w/2 * r
            // y = h/2 * c + h/2 * r
            const int x = mX0 + imgH * c - imgHalfW * r;
            const int y = mY0 + imgHalfH * c + imgHalfH * r;

            if(r == 14 && c == 0)
                std::cout << "[14,0] = " << x << "," << y << std::endl;

            img->SetPosition(x, y);
            img->Render();
        }
    }
}

} // namespace game
