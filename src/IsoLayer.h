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

/// An object of an IsoLayer.
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
    RIGHT           = 0x40,

    // combos
    CENTER          = HCENTER + VCENTER
};

/// A layer of an IsoMap.  Basically It's a container of IsoObjects.
class IsoLayer
{
public:
    IsoLayer(const IsoMap * map, const std::vector<std::string> & files);
    ~IsoLayer();

    bool AddObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment);
    void ClearObject(unsigned int r, unsigned int c);
    bool ReplaceObject(unsigned int r, unsigned int c, int objIndex, ObjectAlignment alignment);
    bool MoveObject(unsigned int r0, unsigned int c0, unsigned int r1, unsigned int c1,
                    ObjectAlignment alignment);
    bool ChangeObject(unsigned int r, unsigned int c, int objIndex);

    void ClearObjects();

    bool IsVisible() const;
    void SetVisible(bool val);
    void Render();

private:
    void AddObject(unsigned int cellIndex, int objIndex, ObjectAlignment alignment);
    void ClearObject(unsigned int index);

    void SetImages(const std::vector<std::string> & files);

private:
    std::vector<lib::graphic::Image *> mImages;

    std::vector<IsoObject *> mObjectsList;
    std::vector<IsoObject *> mObjectsMap;

    const IsoMap * mMap = nullptr;

    bool mVisible = true;
};

// ==================== INLINE METHODS ====================

/**
 * @brief Tells if a layer is visible or not.
 * @return TRUE if layer is visible, FALSE otherwise
 */
inline bool IsoLayer::IsVisible() const { return mVisible; }

/**
 * @brief Sets the visibility status of a layer.
 * @param val Visibility status (TRUE or FALSE)
 */
inline void IsoLayer::SetVisible(bool val) { mVisible = val; }

} // namespace game
