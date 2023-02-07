#pragma once

#include <vector>

namespace game
{

class GameObject;

enum PlayerFaction : unsigned int;

class ControlMap
{
public:
    void SetSize(int rows, int cols);

    void AddControlPointsForObject(GameObject * obj);

    // TEST
    void PrintControlMap();
    void PrintControlMap(PlayerFaction f);

private:
    struct ControlCell
    {
        void UpdateRes();
        void AddPoints(PlayerFaction f, int p);

        std::vector<int> points;
        PlayerFaction res;
    };

private:
    std::vector<ControlCell> mMap;

    unsigned int mRows = 0;
    unsigned int mCols = 0;
};

// ==================== INLINE METHODS ====================

inline void ControlMap::ControlCell::AddPoints(PlayerFaction f, int p)
{
    points[f] += p;
    UpdateRes();
}

} // namespace game
