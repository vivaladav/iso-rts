#include "IsoLayer.h"

#include "IsoMap.h"

#include <graphic/Image.h>

#include <algorithm>
#include <cassert>

namespace game
{

// ==================== CONSTRUCTORS AND DESTRUCTOR ====================

/**
 * @brief Creates a layer of an IsoMap. It's a container of IsoObjects.
 * @param map Pointer to the owner map
 * @param files Files to create images to display in the layer. Paths are relative to binary
 */
IsoLayer::IsoLayer(const IsoMap * map, const std::vector<std::string> & files)
    : mMap(map)
{
    assert(map);

    const int size = mMap->GetNumRows() * mMap->GetNumCols();

    mObjectsMap.reserve(size);
    mObjectsMap.assign(size, nullptr);

    SetImages(files);
}

/// Destructor. delete Images and IsoObjects.
IsoLayer::~IsoLayer()
{
    // images
    for(lib::graphic::Image * img : mImages)
        delete img;

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

/**
 * @brief IsoLayer::AddObject
 * @brief Creates a new IsoObject and adds it to the layer.
 * @param r Row index, starting from 0
 * @param c Col index, starting from 0
 * @param index Position index in the layer. Identifies the cell
 * @param objIndex Object index ID. Identifies the associated image
 * @param alignment Visual alignment of the object in the cell
 * @return TRUE on success, FALSE on error
 */
bool IsoLayer::AddObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment)
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r < rows && c < cols))
        return false;

    const unsigned int index = r * cols + c;

    // ERROR cell full
    if(mObjectsMap[index])
        return false;

    AddObject(index, objIndex, alignment);

    return true;
}

bool IsoLayer::AddObject(unsigned int r, unsigned int c, unsigned int rows, unsigned int cols, int objIndex)
{
    const unsigned int mapRows = mMap->GetNumRows();
    const unsigned int mapCols = mMap->GetNumCols();

    // object origin is out of map
    if(r >= mapRows || c >= mapCols)
        return false;

    // full size is out of map
    const unsigned int r1 = 1 + r - rows;
    const unsigned int c1 = 1 + c - cols;

    if(r1 >= mapRows || c1 >= mapCols)
        return false;

    // create new IsoObject
    IsoObject * obj = new IsoObject({0, 0}, objIndex, rows, cols);

    // position it in a cell
    const lib::core::Point2D cellPos = mMap->GetCellPosition(r, c);
    const int cellH = mMap->GetTileHeight();

    const int x0 = cellPos.x + cellH;
    const int y0 = cellPos.y + cellH;

    const int imgW0 = cols * cellH;
    const int imgH = mImages[obj->imgIndex]->GetHeight();

    obj->pos.x = x0 - imgW0;
    obj->pos.y = y0 - imgH;

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
 * @brief Replaces an object in a cell. First clears the existing one, then creates a new one.
 * @param r Row index, starting from 0
 * @param c Col index, starting from 0
 * @param objIndex Object index ID. Identifies the associated image
 * @param alignment Visual alignment of the object in the cell
 * @return TRUE on success, FALSE otherwise
 */
bool IsoLayer::ReplaceObject(unsigned int r, unsigned int c,
                             int objIndex, ObjectAlignment alignment)
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r < rows && c < cols))
        return false;

    const unsigned int index = r * cols + c;

    if(mObjectsMap[index])
        ClearObject(index);

    AddObject(index, objIndex, alignment);

    return true;
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
                          unsigned int r1, unsigned int c1,
                          ObjectAlignment alignment)
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
    AlignObject(obj, index1, alignment);

    mObjectsMap[index0] = nullptr;
    mObjectsMap[index1] = obj;

    return true;
}

/**
 * @brief Changes the type of an object in a cell.
 * @param r Row index, starting from 0
 * @param c Col index, starting from 0
 * @param objIndex Object index ID. Identifies the associated image
 * @return
 */
bool IsoLayer::ChangeObject(unsigned int r, unsigned int c, int objIndex)
{
    const unsigned int rows = mMap->GetNumRows();
    const unsigned int cols = mMap->GetNumCols();

    // ERROR out of bounds
    if(!(r < rows && c < cols))
        return false;

    const unsigned int index = r * cols + c;

    // ERROR cell empty
    if(!mObjectsMap[index])
        return false;

    mObjectsMap[index]->imgIndex = objIndex;

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
    {
        auto * img = mImages[obj->imgIndex];

        img->SetPosition(obj->pos.x, obj->pos.y);
        img->Render();
    }
}

void IsoLayer::MoveObjects(int deltaX, int deltaY)
{
    for(auto * obj : mObjectsList)
    {
        obj->pos.x += deltaX;
        obj->pos.y += deltaY;
    }
}

// ==================== PRIVATE METHODS ====================

/**
 * @brief Align an IsoObject inside a cell.
 * @param obj Pointer to IsoObject
 * @param cellIndex Index of the target cell in the layer
 * @param alignment Flags that define the alignment
 */
void IsoLayer::AlignObject(IsoObject * obj, unsigned int cellIndex, ObjectAlignment alignment)
{
    // initial position is cell's TL corner
    obj->pos = mMap->GetCellPosition(cellIndex);

    // horizontal alignment
    if(alignment & HCENTER)
        obj->pos.x += (mMap->GetTileWidth() - mImages[obj->imgIndex]->GetWidth()) * 0.5f;
    else if(alignment & RIGHT)
        obj->pos.x += mMap->GetTileWidth() - mImages[obj->imgIndex]->GetWidth();

    // vertical alignment
    if(alignment & VCENTER)
        obj->pos.y += (mMap->GetTileHeight() - mImages[obj->imgIndex]->GetHeight()) * 0.5f;
    else if(alignment & BOTTOM)
        obj->pos.y += mMap->GetTileHeight() - mImages[obj->imgIndex]->GetHeight();
}

/**
 * @brief Creates a new IsoObject and adds it to the layer.
 * @param index Position index in the layer. Identifies the cell
 * @param objIndex Object index ID. Identifies the associated image
 * @param alignment Visual alignment of the object in the cell
 */
void IsoLayer::AddObject(unsigned int cellIndex, int objIndex, ObjectAlignment alignment)
{
    // create new IsoObject
    IsoObject * obj = new IsoObject({0, 0}, objIndex);

    // position it in a cell
    AlignObject(obj, cellIndex, alignment);

    // store object
    mObjectsMap[cellIndex] = obj;
    mObjectsList.emplace_back(obj);
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

/**
 * @brief Creates the Images used to represent the object in the layer.
 * @param files Files to create images. Paths are relative to binary
 */
void IsoLayer::SetImages(const std::vector<std::string> & files)
{
    for(const std::string & file : files)
    {
        auto * img = new lib::graphic::Image(file.c_str());

        mImages.emplace_back(img);
    }
}

} // namespace game
