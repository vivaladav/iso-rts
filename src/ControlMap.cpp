#include "ControlMap.h"

#include "GameConstants.h"
#include "Player.h"
#include "GameObjects/GameObject.h"

#include <iostream>

namespace game
{

void ControlMap::SetSize(int rows, int cols)
{
    const int size = rows * cols;

    mRows = rows;
    mCols = cols;

    const ControlCell cc0 = { {0, 0, 0}, NO_FACTION };
    mMap.assign(size, cc0);
}

void ControlMap::AddControlPointsForObject(GameObject * obj)
{
    const PlayerFaction faction = obj->GetOwner()->GetFaction();

    const int rTL = obj->GetRow1();
    const int cTL = obj->GetCol1();
    const int rBR = obj->GetRow0();
    const int cBR = obj->GetCol0();

    // TODO get max points from object
    const int maxPoints = 5;

    // assign points to object area
    for(int r = rTL; r <= rBR; ++r)
    {
        const int ind0 = r * mCols;

        for(int c = cTL; c <= cBR; ++c)
        {
            const int ind = ind0 + c;
            mMap[ind].AddPoints(faction, maxPoints);
        }
    }

    // assign descending points
    for(int i = 1; i < maxPoints; ++i)
    {
        const int p = maxPoints - i;

        // TOP ROW
        const int rT = rTL - i;
        const int c0 = cTL - i >= 0 ? cTL - i : 0;
        const int c1 = cBR + i < mCols ? cBR + i : mCols - 1;

        if(rT >= 0)
        {
            const int ind0 = rT * mCols;

            for(int c = c0; c <= c1; ++c)
            {
                const int ind = ind0 + c;
                mMap[ind].AddPoints(faction, p);
            }
        }

        // BOTTOM ROW
        const int rB = rBR + i;

        if(rB < mRows)
        {
            const int ind0 = rB * mCols;

            for(int c = c0; c <= c1; ++c)
            {
                const int ind = ind0 + c;
                mMap[ind].AddPoints(faction, p);
            }
        }

        // LEFT COL
        const int cL = cTL - i;
        const int r0 = rTL - i + 1 >= 0 ? rTL - i + 1 : 0;
        const int r1 = rBR + i - 1 < mRows ? rBR + i - 1 : mRows - 1;

        if(cL >= 0)
        {
            for(int r = r0; r <= r1; ++r)
            {
                const int ind = (r * mCols) + cL;
                mMap[ind].AddPoints(faction, p);
            }
        }

        // RIGHT COL
        const int cR = cBR + i;

        if(cR < mCols)
        {
            for(int r = r0; r <= r1; ++r)
            {
                const int ind = (r * mCols) + cR;
                mMap[ind].AddPoints(faction, p);
            }
        }
    }
}

void ControlMap::PrintControlMap()
{
    for(unsigned int r = 0; r < mRows; ++r)
    {
        const int ind0 = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const int ind = ind0 + c;
            std::cout << mMap[ind].res;
        }

        std::cout << "\n";
    }

    std::cout << std::endl;
}

void ControlMap::PrintControlMap(PlayerFaction f)
{
    for(unsigned int r = 0; r < mRows; ++r)
    {
        const int ind0 = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const int ind = ind0 + c;
            std::cout << mMap[ind].points[f];
        }

        std::cout << "\n";
    }

    std::cout << std::endl;
}

void ControlMap::ControlCell::UpdateRes()
{
    int maxPoints = 0;

    res = NO_FACTION;

    for(unsigned int f = FACTION_1; f < NUM_FACTIONS; ++f)
    {
        if(points[f] > maxPoints)
        {
            maxPoints = points[f];
            res = static_cast<PlayerFaction>(f);
        }
    }
}

} // namespace game
