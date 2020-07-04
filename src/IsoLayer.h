#pragma once

#include <core/Point.h>

#include <string>
#include <vector>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class IsoMap;

struct IsoObject
{
    IsoObject(int ind, const lib::core::Point2D & p)
        : imgIndex(ind)
        , pos(p)
    {
    }

    int imgIndex;
    lib::core::Point2D pos;
};

enum ObjectAlignment
{
    NO_ALIGNMENT    = 0,

    // vertical alignment
    TOP             = 0x01,
    VCENTER         = 0x02,
    BOTTOM          = 0x04,

    // horizontal alignment
    LEFT            = 0x10,
    HCENTER         = 0x20,
    RIGHT           = 0x40
};

class IsoLayer
{
public:
    IsoLayer(const IsoMap * map, const std::vector<std::string> & files);
    ~IsoLayer();

    bool AddObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment);
    void ClearObject(unsigned int r, unsigned int c);
    bool ReplaceObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment);

    void Render();

private:
    void AddObject(unsigned int index, int objIndex, ObjectAlignment alignment);
    void ClearObject(unsigned int index);

    void SetImages(const std::vector<std::string> & files);

private:
    std::vector<lib::graphic::Image *> mImages;

    std::vector<IsoObject *> mObjectsList;
    std::vector<IsoObject *> mObjectsMap;

    const IsoMap * mMap = nullptr;
};

} // namespace game
