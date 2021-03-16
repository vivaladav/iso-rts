#pragma once

#include <core/Point.h>

#include <string>
#include <vector>

namespace game
{

class IsoMap;
class IsoObject;

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
    IsoLayer(const IsoMap * map);

    void UpdateSize();

    bool ContainsObject(unsigned int r, unsigned int c) const;
    bool AddObject(IsoObject * obj, unsigned int r, unsigned int c);
    void ClearObject(IsoObject * obj);
    void ClearObject(unsigned int r, unsigned int c);
    bool MoveObject(unsigned int r0, unsigned int c0,
                    unsigned int r1, unsigned int c1,
                    bool updatePosition = true);

    void SetObjectVisible(IsoObject * obj, bool visible);

    void ClearObjects();

    bool IsVisible() const;
    void SetVisible(bool val);
    void Render();

    void MoveObjectsPosition(int deltaX, int deltaY);

    void RepositionObject(IsoObject * obj);

    lib::core::Pointd2D GetObjectPosition(const IsoObject * obj, unsigned int r, unsigned int c) const;

private:
    void PositionObject(IsoObject * obj, unsigned int r, unsigned int c);

    void ClearObject(unsigned int index);

    void RemoveObjectFromList(IsoObject * obj);

    void RemoveObjectFromRenderList(IsoObject * obj);
    void InsertObjectInRenderList(IsoObject * obj);

    void ClearObjectFromMap(IsoObject * obj);
    void InsertObjectInMap(IsoObject * obj);

private:
    std::vector<IsoObject *> mObjectsList;
    std::vector<IsoObject *> mRenderList;
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
