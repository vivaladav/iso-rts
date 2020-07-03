#include "IsoLayer.h"

#include "IsoMap.h"

#include <graphic/Image.h>

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

    lib::core::Point2D posCell = mMap->GetCellPosition(r, c);

    // TODO alignment

    IsoObject * obj = new IsoObject(objIndex, posCell);

    mObjectsMap[index] = obj;

    mObjectsList.emplace_back(obj);

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

void IsoLayer::SetImages(const std::vector<std::string> & files)
{
    for(const std::string & file : files)
    {
        auto * img = new lib::graphic::Image(file.c_str());

        mImages.emplace_back(img);
    }
}

} // namespace game
