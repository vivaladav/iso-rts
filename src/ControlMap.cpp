#include "ControlMap.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"
#include "IsoLayer.h"
#include "Player.h"
#include "GameObjects/GameObject.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Texture.h>

#include <iostream>

namespace game
{

ControlMap::ControlMap(IsoLayer * layer)
    : mLayer(layer)
{
    // init all 2^8 combinations to error type
    const int typeCombinations = 256;
    mTypesMap.assign(typeCombinations, IND_INF_AREA_UNDEFINED);

    unsigned int ind;

    // CORNER BL
    ind = CN_T | CN_TR | CN_R;
    mTypesMap[ind] = IND_INF_AREA_BL_F1;

    ind = CN_ALL_T | CN_R;
    mTypesMap[ind] = IND_INF_AREA_BL_F1;

    ind = CN_ALL_R | CN_T;
    mTypesMap[ind] = IND_INF_AREA_BL_F1;

    ind = CN_ALL_R | CN_ALL_T;
    mTypesMap[ind] = IND_INF_AREA_BL_F1;

    // CORNER BR
    ind = CN_T | CN_TL | CN_L;
    mTypesMap[ind] = IND_INF_AREA_BR_F1;

    ind = CN_ALL_L | CN_T;
    mTypesMap[ind] = IND_INF_AREA_BR_F1;

    ind = CN_ALL_T | CN_L;
    mTypesMap[ind] = IND_INF_AREA_BR_F1;

    ind = CN_ALL_T | CN_ALL_L;
    mTypesMap[ind] = IND_INF_AREA_BR_F1;

    // CORNER TL
    ind = CN_R | CN_BR | CN_B;
    mTypesMap[ind] = IND_INF_AREA_TL_F1;

    ind = CN_R | CN_ALL_B;
    mTypesMap[ind] = IND_INF_AREA_TL_F1;

    ind = CN_B | CN_ALL_R;
    mTypesMap[ind] = IND_INF_AREA_TL_F1;

    ind = CN_ALL_B | CN_ALL_R;
    mTypesMap[ind] = IND_INF_AREA_TL_F1;

    // CORNER TR
    ind = CN_L | CN_BL | CN_B;
    mTypesMap[ind] = IND_INF_AREA_TR_F1;

    ind = CN_L | CN_ALL_B;
    mTypesMap[ind] = IND_INF_AREA_TR_F1;

    ind = CN_B | CN_ALL_L;
    mTypesMap[ind] = IND_INF_AREA_TR_F1;

    ind = CN_ALL_B | CN_ALL_L;
    mTypesMap[ind] = IND_INF_AREA_TR_F1;

    // LEFT
    ind = CN_ALL_R | CN_T | CN_B;
    mTypesMap[ind] = IND_INF_AREA_L_F1;

    ind = CN_ALL_R | CN_ALL_T | CN_B;
    mTypesMap[ind] = IND_INF_AREA_L_F1;

    ind = CN_ALL_R | CN_ALL_B | CN_T;
    mTypesMap[ind] = IND_INF_AREA_L_F1;

    ind = CN_ALL_R | CN_ALL_B | CN_ALL_T;
    mTypesMap[ind] = IND_INF_AREA_L_F1;

    // RIGHT
    ind = CN_ALL_L | CN_T | CN_B;
    mTypesMap[ind] = IND_INF_AREA_R_F1;

    ind = CN_ALL_L | CN_ALL_T | CN_B;
    mTypesMap[ind] = IND_INF_AREA_R_F1;

    ind = CN_ALL_L | CN_ALL_B | CN_T;
    mTypesMap[ind] = IND_INF_AREA_R_F1;

    ind = CN_ALL_L | CN_ALL_B | CN_ALL_T;
    mTypesMap[ind] = IND_INF_AREA_R_F1;

    // TOP
    ind = CN_L | CN_R | CN_ALL_B;
    mTypesMap[ind] = IND_INF_AREA_T_F1;

    ind = CN_ALL_L | CN_ALL_B | CN_R;
    mTypesMap[ind] = IND_INF_AREA_T_F1;

    ind = CN_ALL_R | CN_ALL_B | CN_L;
    mTypesMap[ind] = IND_INF_AREA_T_F1;

    ind = CN_ALL_R | CN_ALL_B | CN_ALL_L;
    mTypesMap[ind] = IND_INF_AREA_T_F1;

    // BOTTOM
    ind = CN_L | CN_R | CN_ALL_T;
    mTypesMap[ind] = IND_INF_AREA_B_F1;

    ind = CN_ALL_L | CN_ALL_T | CN_R;
    mTypesMap[ind] = IND_INF_AREA_B_F1;

    ind = CN_ALL_R | CN_ALL_T | CN_L;
    mTypesMap[ind] = IND_INF_AREA_B_F1;

    ind = CN_ALL_R | CN_ALL_T | CN_ALL_L;
    mTypesMap[ind] = IND_INF_AREA_B_F1;

    // CONNECTOR LEFT
    ind = CN_ALL_R | CN_ALL_T | CN_L | CN_B;
    mTypesMap[ind] = IND_INF_AREA_CONN_L_F1;

    // CONNECTOR RIGHT
    ind = CN_ALL_L | CN_ALL_B | CN_R | CN_T;
    mTypesMap[ind] = IND_INF_AREA_CONN_R_F1;

    // CONNECTOR BOTTOM
    ind = CN_ALL_L | CN_ALL_T | CN_R | CN_B;
    mTypesMap[ind] = IND_INF_AREA_CONN_B_F1;

    // CONNECTOR TOP
    ind = CN_ALL_R | CN_ALL_B | CN_L | CN_T;
    mTypesMap[ind] = IND_INF_AREA_CONN_T_F1;
}

void ControlMap::SetSize(unsigned int rows, unsigned int cols)
{
    const unsigned int size = rows * cols;

    mRows = rows;
    mCols = cols;

    const ControlCell cc0 = { {0, 0, 0}, NO_FACTION };
    mMap.assign(size, cc0);
}

void ControlMap::AddControlPointsForCell(unsigned int r, unsigned int c, PlayerFaction faction)
{
    const int maxPoints = 3;

    AddControlPointstoArea(r, c, r, c, faction, maxPoints);
}

void ControlMap::AddControlPointsForObject(GameObject * obj)
{
    const int rTL = obj->GetRow1();
    const int cTL = obj->GetCol1();
    const int rBR = obj->GetRow0();
    const int cBR = obj->GetCol0();

    const PlayerFaction faction = obj->GetOwner()->GetFaction();

    // TODO get max points from object
    const int maxPoints = 7;

    AddControlPointstoArea(rTL, cTL, rBR, cBR, faction, maxPoints);
}

void ControlMap::AddControlPointstoArea(int rTL, int cTL,
                                        int rBR, int cBR,
                                        PlayerFaction faction, int maxPoints)
{
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

void ControlMap::UpdateVisualAreas()
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();

    mLayer->ClearObjects();

    ClearUsedMarkers();

    for(unsigned int r = 0; r < mRows; ++r)
    {
        const int ind0 = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const int ind = ind0 + c;
            const PlayerFaction pf = mMap[ind].controller;

            if(pf != NO_FACTION)
            {
                const unsigned int type = GetMarkerType(r, c, pf);

                if(type != IND_INF_AREA_UNDEFINED)
                {
                    const unsigned int typef = type + (NUM_INF_AREA_ELEMS * pf);

                    IsoObject * marker = GetNewMarker();
                    marker->SetTexture(tm->GetSprite(SpriteFileIndicators, typef));

                    mLayer->AddObject(marker, r, c);
                }
            }
        }
    }
}

void ControlMap::PrintControlMap() const
{
    for(unsigned int r = 0; r < mRows; ++r)
    {
        const int ind0 = r * mCols;

        for(unsigned int c = 0; c < mCols; ++c)
        {
            const int ind = ind0 + c;
            std::cout << mMap[ind].controller;
        }

        std::cout << "\n";
    }

    std::cout << std::endl;
}

void ControlMap::PrintControlMap(PlayerFaction f) const
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

    controller = NO_FACTION;

    for(unsigned int f = FACTION_1; f < NUM_FACTIONS; ++f)
    {
        if(points[f] > maxPoints)
        {
            maxPoints = points[f];
            controller = static_cast<PlayerFaction>(f);
        }
    }
}

IsoObject * ControlMap::GetNewMarker()
{
    IsoObject * marker = nullptr;

    if(mUsedMarkers < mMarkers.size())
        marker = mMarkers[mUsedMarkers];
    else
    {
        marker = new IsoObject(1, 1);
        mMarkers.push_back(marker);
    }

    ++mUsedMarkers;

    return marker;
}

unsigned int ControlMap::GetMarkerType(unsigned int r, unsigned int c, PlayerFaction f) const
{
    const unsigned int ind = FactionOwnsCell(r - 1, c - 1, f) * CN_TL +
                             FactionOwnsCell(r - 1, c, f) * CN_T +
                             FactionOwnsCell(r - 1, c + 1, f) * CN_TR +
                             FactionOwnsCell(r, c - 1, f) * CN_L +
                             FactionOwnsCell(r, c + 1, f) * CN_R +
                             FactionOwnsCell(r + 1, c - 1, f) * CN_BL +
                             FactionOwnsCell(r + 1, c, f) * CN_B +
                             FactionOwnsCell(r + 1, c + 1, f) * CN_BR;

    return mTypesMap[ind];
}

bool ControlMap::FactionOwnsCell(unsigned int r, unsigned int c, PlayerFaction f) const
{
    if(r >= mRows || c >= mCols)
        return false;

    const int ind = (r * mCols) + c;
    return mMap[ind].controller == f;
}

} // namespace game
