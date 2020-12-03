#include "IsoMap.h"

#include "Cell2D.h"
#include "IsoLayer.h"

#include <graphic/Image.h>

#include <algorithm>
#include <fstream>

namespace game
{

// ==================== CONSTRUCTORS AND DESTRUCTOR ====================

constexpr unsigned int MAX_SIZE = 256;

/**
 * @brief Creates an isometric map loading a file.
 * @param file Map file to load. Path relative to application binary
 * @param tileW Width of a cell (height is half of that)
 */
IsoMap::IsoMap(const char * file, int tileW)
    : mTileW(tileW)
    , mTileH(tileW * 0.5f)
{
    Load(file);
}

/// Destructor, deletes Images used for the tiles and IsoLayers.
IsoMap::~IsoMap()
{
    for(lib::graphic::Image * img : mTiles)
        delete img;

    // -- IsoLayers --
    for(IsoLayer * layer : mLayers)
        delete layer;
}

// ==================== PUBLIC METHODS ====================

/**
 * @brief Loads a map file.
 * @param file Map file to load. Path relative to application binary
 * @return TRUE on success, FALSE otherwise
 */
bool IsoMap::Load(const char * file)
{
    // open map file
    std::fstream f(file);

    if(!f.is_open())
        return false;

    // clear internal data
    mMap.clear();
    mTilePositions.clear();

    // read map file
    mMap.reserve(MAX_SIZE);

    std::string line;

    mRows = 0;

    while (std::getline(f, line))
    {
        mCols = line.length();

        for(unsigned int c = 0; c < mCols; ++c)
           mMap.push_back(line[c] - '0');

        ++mRows;
    }

    // update tile positions data
    const unsigned int mapSize = mRows * mCols;

    mTilePositions.reserve(mapSize);
    mTilePositions.assign(mapSize, lib::core::Point2D(0, 0));

    UpdateTilePositions();

    return true;
}

/**
 * @brief Gives the top-left corner position of a cell.
 * @param index Position index in the map. Identifies the cell
 * @return A Point2D struct containing the (x,y) coordinates of the cell
 */
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

/**
 * @brief Creates the Images used to represent the tiles in the map.
 * @param files Files to create images. Paths are relative to binary
 */
void IsoMap::SetTiles(const std::vector<std::string> & files)
{
    for(const std::string & file : files)
    {
        auto * img = new lib::graphic::Image(file.c_str());

        mTiles.emplace_back(img);
    }

    UpdateTilePositions();
}

/// Renders all the cells in the map and on top of that all the IsoLayers.
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
    for(IsoLayer * layer : mLayersRenderList)
        layer->Render();
}

/**
 * @brief Identifies the cell corresponding to a (x,y) screen position.
 * @param x X coordinate of the position
 * @param y Y coordinate of the position
 * @return A Cell2D struct containing a (row,col) pair that identifies the correspongind cell
 */
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

/**
 * @brief Checks if a cell is inside the map borders.
 * @param cell Struct containing the row,col coordinates of the cell
 * @return TRUE if the cell is inside the map, FALSE otherwise
 */
bool IsoMap::IsCellInside(const Cell2D & cell) const
{
    const unsigned int cr = static_cast<unsigned int>(cell.row);
    const unsigned int cc = static_cast<unsigned int>(cell.col);
    return  cr < mRows && cc < mCols;
}

/**
 * @brief Creates and stores an IsoLayer.
 * @param layerId Layer ID
 * @param files Files to create images. Paths are relative to binary
 * @return A pointer to the new IsoLayer
 */
IsoLayer * IsoMap::CreateLayer(unsigned int layerId, const std::vector<std::string> & files)
{
    // check layer has not been created yet
    IsoLayer * layer = GetLayer(layerId);

    if(layer != nullptr)
        return layer;

    // create and store new layer
    layer = new IsoLayer(this, files);

    mLayers.emplace_back(layer);
    mLayersMap.insert({layerId, layer});
    mLayersRenderList.emplace_back(layer);

    return layer;
}

/**
 * @brief Makes an IsoLayer visible or invisible.
 * @param layerId Layer ID
 * @param visible TRUE if you want to make the layer visible, FALSE otherwise
 */
void IsoMap::SetLayerVisible(unsigned int layerId, bool visible)
{
    IsoLayer * layer = GetLayer(layerId);

    // layer not found
    if(nullptr == layer)
        return ;

    // nothing to do
    if(layer->IsVisible() == visible)
        return ;

    layer->SetVisible(visible);

    if(!visible)
    {
        auto it = std::find(mLayersRenderList.begin(), mLayersRenderList.end(), layer);

        if(it != mLayersRenderList.end())
            mLayersRenderList.erase(it);
    }
    else
    {
        // layer should be first
        if(layer == mLayers.front())
            mLayersRenderList.insert(mLayersRenderList.begin(), layer);
        // layer should be last
        else if(layer == mLayers.back())
            mLayersRenderList.emplace_back(layer);
        // layer should be in the middle
        else
        {
            for(unsigned int i = 1; i < (mLayers.size() - 1); ++i)
            {
                if(mLayers[i] == layer)
                {
                    mLayersRenderList.insert(mLayersRenderList.begin() + i, layer);
                    break;
                }
            }
        }
    }
}

// ==================== PRIVATE METHODS ====================

/// Updates the position of all the tiles. Based on tile size and map origin.
void IsoMap::UpdateTilePositions()
{
    // no positions to update yet
    if(mTilePositions.empty())
        return ;

     const int tileHalfH = mTileH * 0.5f;

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
            p.y = mY0 + tileHalfH * (c + r);
        }
    }
}

} // namespace game
