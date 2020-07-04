#include "IsoMap.h"

#include "Cell2D.h"
#include "IsoLayer.h"

#include <graphic/Image.h>

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
    mTilePositions.assign(size, lib::core::Point2D(0, 0));

    mMap.assign(size, 0);
}

IsoMap::~IsoMap()
{
    for(lib::graphic::Image * img : mTiles)
        delete img;

    // -- IsoLayers --
    for(IsoLayer * layer : mLayers)
        delete layer;
}

lib::core::Point2D IsoMap::GetCellPosition(unsigned int index) const
{
    if(index < mTilePositions.size())
        return mTilePositions[index];
    else
    {
        const lib::core::Point2D p(-1, -1);
        return p;
    }
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

    // -- IsoLayers --
    for(IsoLayer * layer : mLayers)
        layer->Render();
}

Cell2D IsoMap::CellFromScreenPoint(int x, int y) const
{
    const float xf = x - mX0;
    const float yf = y - mY0;
    const float w = mTileW;
    const float h = mTileH;

    float col = (2. * yf + xf) / w;
    float row = col - (xf / h);

    // negative cells need special handling
    if(row < 0.f)
        row -= 1.f;

    if(col < 0.f)
        col -= 1.f;

    const Cell2D cell(static_cast<int>(row), static_cast<int>(col));
    return cell;
}


bool IsoMap::IsCellInside(const Cell2D & cell) const
{
    const unsigned int cr = static_cast<unsigned int>(cell.row);
    const unsigned int cc = static_cast<unsigned int>(cell.col);
    return  cr < mRows && cc < mCols;
}

void IsoMap::CreateIsoLayer(const std::vector<std::string> & files)
{
    IsoLayer * layer = new IsoLayer(this, files);

    mLayers.emplace_back(layer);
}

void IsoMap::UpdateTilePositions()
{
    // TODO consider image size of the different tiles
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
