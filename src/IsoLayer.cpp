#include "IsoLayer.h"

#include "IsoMap.h"
#include "IsoObject.h"

#include <algorithm>

namespace game
{

// ==================== CONSTRUCTORS AND DESTRUCTOR ====================

/**
 * @brief Creates a layer of an IsoMap. It's a container of IsoObjects.
 * @param map Pointer to the owner map
 * @param files Files to create images to display in the layer. Paths are relative to binary
 */
IsoLayer::IsoLayer(const IsoMap * map)
    : mMap(map)
{
    const int size = mMap->GetNumRows() * mMap->GetNumCols();

    mObjectsMap.reserve(size);
    mObjectsMap.assign(size, nullptr);
}

/// Destructor. delete Images and IsoObjects.
IsoLayer::~IsoLayer()
{
    // objects
    ClearObjects();
}

// ==================== PUBLIC METHODS ====================

void IsoLayer::UpdateSize()
{
    const unsigned int size = mMap->GetNumRows() * mMap->GetNumCols();

    if(size == mObjectsMap.size())
        return ;

    mObjectsMap.resize(size);

    // update objects list
    mObjectsList.clear();

    for(unsigned int ind = 0; ind < size; ++ind)
    {
        if(mObjectsMap[ind] != nullptr)
            mObjectsList.emplace_back(mObjectsMap[ind]);
    }
}

/**
 * @brief Checks if a cell contains an object.
 * @param r Row index, starting from 0
 * @param c Col index, starting from 0
 * @return TRUE if cell contains an object, FALSE otherwise
 */
bool IsoLayer::ContainsObject(unsigned int r, unsigned int c) const
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r < rows && c < cols))
        return false;

    const unsigned int index = r * cols + c;

    return mObjectsMap[index] != nullptr;
}

bool IsoLayer::AddObject(IsoObject * obj, unsigned int r, unsigned int c)
{
    const unsigned int mapRows = mMap->GetNumRows();
    const unsigned int mapCols = mMap->GetNumCols();

    // object origin is out of map
    if(r >= mapRows || c >= mapCols)
        return false;

    // full size is out of map
    const unsigned int r1 = 1 + r - obj->GetRows();
    const unsigned int c1 = 1 + c - obj->GetCols();

    if(r1 >= mapRows || c1 >= mapCols)
        return false;

    // position it in a cell
    PositionObject(obj, r, c);

    // store object
    for(unsigned int row = r1; row <= r; ++row)
    {
        const unsigned int indBase = row * mapCols;

        for(unsigned int col = c1; col <= c; ++col)
        {
            const unsigned int ind = indBase + col;
            mObjectsMap[ind] = obj;
        }
    }

    mObjectsList.emplace_back(obj);

    return true;
}

/**
 * @brief Destroys an object in a cell.
 * @param r Row index, starting from 0
 * @param c Col index, starting from 0
 */
void IsoLayer::ClearObject(unsigned int r, unsigned int c)
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r < rows && c < cols))
        return;

    const unsigned int index = r * cols + c;

    // empty cell -> nothing to do
    if(!mObjectsMap[index])
        return ;

    ClearObject(index);
}

/**
 * @brief Moves an object from one cell to another.
 * @param r0 Row index of start cell, starting from 0
 * @param c0 Col index of start cell, starting from 0
 * @param r1 Row index of dest cell, starting from 0
 * @param c1 Col index of start cell, starting from 0
 * @param alignment Visual alignment of the object in the cell
 * @return TRUE on success, FALSE otherwise
 */
bool IsoLayer::MoveObject(unsigned int r0, unsigned int c0,
                          unsigned int r1, unsigned int c1)
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r0 < rows && c0 < cols && r1 < rows && c1 < cols))
        return false;

    const unsigned int index0 = r0 * cols + c0;

    IsoObject * obj = mObjectsMap[index0];

    // nothing to move
    if(!obj)
        return false;

    const unsigned int index1 = r1 * cols + c1;

    // cell is full
    if(mObjectsMap[index1])
        return false;

    // position object
    PositionObject(obj, r1, c1);

    mObjectsMap[index0] = nullptr;
    mObjectsMap[index1] = obj;

    return true;
}

/// Destroy all the objects
void IsoLayer::ClearObjects()
{
    // delete objects
    for(IsoObject * obj : mObjectsList)
        delete obj;

    // clear lists
    mObjectsList.clear();

    const unsigned int mapsSize = mMap->GetNumRows() * mMap->GetNumCols();
    mObjectsMap.assign(mapsSize, nullptr);
}

/// Renders all the objects in the layer.
void IsoLayer::Render()
{
    for(auto * obj : mObjectsList)
        obj->Render();
}

void IsoLayer::MoveObjects(int deltaX, int deltaY)
{
    for(auto * obj : mObjectsList)
    {
        const int x = obj->GetX() + deltaX;
        const int y = obj->GetY() + deltaY;

        obj->SetPosition(x, y);
    }
}

// ==================== PRIVATE METHODS ====================

void IsoLayer::PositionObject(IsoObject * obj, unsigned int r, unsigned int c)
{
    const lib::core::Point2D cellPos = mMap->GetCellPosition(r, c);
    const int cellH = mMap->GetTileHeight();

    const int x0 = cellPos.x + cellH;
    const int y0 = cellPos.y + cellH;

    const int imgW0 = obj->GetCols() * cellH;
    const int imgH = obj->GetHeight();

    obj->SetPosition(x0 - imgW0, y0 - imgH);
}

/**
 * @brief Destroys an object from a cell.
 * @param index Position index in the layer. Identifies the cell
 */
void IsoLayer::ClearObject(unsigned int index)
{
    IsoObject * obj = mObjectsMap[index];
    mObjectsMap[index] = nullptr;

    auto it = std::find(mObjectsList.begin(), mObjectsList.end(), obj);

    if(it != mObjectsList.end())
        mObjectsList.erase(it);

    delete obj;
}

} // namespace game
