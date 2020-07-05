#include "IsoLayer.h"

#include "IsoMap.h"

#include <graphic/Image.h>

#include <algorithm>
#include <cassert>

namespace game
{

IsoLayer::IsoLayer(const IsoMap * map, const std::vector<std::string> & files)
    : mMap(map)
{
    assert(map);

    const int size = mMap->GetNumRows() * mMap->GetNumCols();

    mObjectsMap.reserve(size);
    mObjectsMap.assign(size, nullptr);

    SetImages(files);
}

IsoLayer::~IsoLayer()
{
    // images
    for(lib::graphic::Image * img : mImages)
        delete img;

    // objects
    for(IsoObject * obj : mObjectsList)
        delete obj;
}

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

bool IsoLayer::ReplaceObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment)
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

void IsoLayer::Render()
{
    for(auto * obj : mObjectsList)
    {
        auto * img = mImages[obj->imgIndex];

        img->SetPosition(obj->pos.x, obj->pos.y);

        img->Render();
    }
}

void IsoLayer::AddObject(unsigned int index, int objIndex, ObjectAlignment alignment)
{
    lib::core::Point2D posCell = mMap->GetCellPosition(index);

    // TODO alignment

    IsoObject * obj = new IsoObject(objIndex, posCell);

    mObjectsMap[index] = obj;

    mObjectsList.emplace_back(obj);
}

void IsoLayer::ClearObject(unsigned int index)
{
    IsoObject * obj = mObjectsMap[index];
    mObjectsMap[index] = nullptr;

    auto it = std::find(mObjectsList.begin(), mObjectsList.end(), obj);

    if(it != mObjectsList.end())
        mObjectsList.erase(it);

    delete obj;
}

void IsoLayer::SetImages(const std::vector<std::string> & files)
{
    for(const std::string & file : files)
    {
        auto * img = new lib::graphic::Image(file.c_str());

        mImages.emplace_back(img);
    }
}

} // namespace game
