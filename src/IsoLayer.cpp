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
    for(IsoObject * obj : mObjectsList)
        delete obj;
}

// ==================== PUBLIC METHODS ====================

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

    // TODO alignment
    obj->pos = mMap->GetCellPosition(index1);

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

// ==================== PRIVATE METHODS ====================

/**
 * @brief Creates a new IsoObject and adds it to the layer.
 * @param index Position index in the layer. Identifies the cell
 * @param objIndex Object index ID. Identifies the associated image
 * @param alignment Visual alignment of the object in the cell
 */
void IsoLayer::AddObject(unsigned int index, int objIndex, ObjectAlignment alignment)
{
    lib::core::Point2D posCell = mMap->GetCellPosition(index);

    // TODO alignment

    IsoObject * obj = new IsoObject(objIndex, posCell);

    mObjectsMap[index] = obj;

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
