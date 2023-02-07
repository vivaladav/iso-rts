#pragma once

#include <vector>

namespace game
{

class GameObject;
class IsoLayer;
class IsoObject;

enum PlayerFaction : unsigned int;

class ControlMap
{
public:
    ControlMap(IsoLayer * layer);
    void SetSize(int rows, int cols);

    void AddControlPointsForObject(GameObject * obj);

    void UpdateVisualAreas();

    // TEST
    void PrintControlMap() const;
    void PrintControlMap(PlayerFaction f) const;

private:
    IsoObject * GetNewMarker();
    void ClearUsedMarkers();

    unsigned int GetMarkerType(int r, int c, PlayerFaction f) const;
    bool FactionOwnsCell(int r, int c, PlayerFaction f) const;

private:
    struct ControlCell
    {
        void UpdateRes();
        void AddPoints(PlayerFaction f, int p);

        std::vector<int> points;
        PlayerFaction controller;
    };

    enum CellNeighbors : unsigned int
    {
        CN_TL    = 0x01,
        CN_T     = 0x02,
        CN_TR    = 0x04,
        CN_R     = 0x08,
        CN_BR    = 0x10,
        CN_B     = 0x20,
        CN_BL    = 0x40,
        CN_L     = 0x80,

        CN_TOP = CN_TL + CN_T + CN_TR,
        CN_BOT = CN_BL + CN_B + CN_BR,
        CN_ALL_L = CN_TL + CN_L + CN_BL,
        CN_ALL_R = CN_TR + CN_R + CN_BR,
        CN_ALL = CN_TOP + CN_BOT + CN_L + CN_R
    };

private:
    std::vector<ControlCell> mMap;
    std::vector<IsoObject * > mMarkers;
    std::vector<unsigned int> mTypesMap;
    unsigned int mUsedMarkers = 0;

    IsoLayer * mLayer = nullptr;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

// ==================== INLINE METHODS ====================

inline void ControlMap::ClearUsedMarkers() { mUsedMarkers = 0; }

inline void ControlMap::ControlCell::AddPoints(PlayerFaction f, int p)
{
    points[f] += p;
    UpdateRes();
}

} // namespace game
