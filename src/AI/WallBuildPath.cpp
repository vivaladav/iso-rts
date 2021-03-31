#include "AI/WallBuildPath.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "IsoLayer.h"
#include "IsoMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Unit.h"
#include "Indicators/WallIndicator.h"
#include "Screens/ScreenGame.h"

#include <cmath>

namespace game
{

WallBuildPath::~WallBuildPath()
{
    // delete the ConquestIndicators
    for(auto ind : mIndicators)
        delete ind;
}

void WallBuildPath::CreateIndicators()
{
    IsoLayer * layer = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);

    Player * player = mObj->GetOwner();

    const PlayerFaction faction = player->GetFaction();

    std::vector<Cell2D> cellsPath;
    cellsPath.reserve(mCells.size());

    // store coordinates of start cell
    const unsigned int pathInd0 = mCells[0];
    const unsigned int indRow0 = pathInd0 / mIsoMap->GetNumCols();
    const unsigned int indCol0 = pathInd0 % mIsoMap->GetNumCols();
    cellsPath.emplace_back(indRow0, indCol0);

    for(unsigned int i = 1; i < mCells.size(); ++i)
    {
        auto ind = new WallIndicator;
        mIndicators.emplace_back(ind);

        // add indicator to layer
        const unsigned int pathInd = mCells[i];
        const unsigned int indRow = pathInd / mIsoMap->GetNumCols();
        const unsigned int indCol = pathInd % mIsoMap->GetNumCols();
        cellsPath.emplace_back(indRow, indCol);

        layer->AddObject(ind, indRow, indCol);

        ind->SetFaction(faction);
    }

    SetIndicatorsType(cellsPath, mIndicators);
}

void WallBuildPath::InitNextBuild()
{
    mState = BUILDING;
    while(mNextCell < mCells.size())
    {
        const unsigned int nextInd = mCells[mNextCell];
        const unsigned int nextRow = nextInd / mIsoMap->GetNumCols();
        const unsigned int nextCol = nextInd % mIsoMap->GetNumCols();
        const Cell2D nextCell(nextRow, nextCol);

        Player * player = mObj->GetOwner();

        // check if conquest is possible
        if(!mGameMap->CanBuildWall(nextCell, player))
        {
            ++mNextCell;

            continue;
        }

        // start conquest
        mGameMap->StartBuildWall(nextCell, player);

        // clear indicator before starting construction
        const int indexInd = mNextCell - 1;

        IsoLayer * layerOverlay = mIsoMap->GetLayer(MapLayers::CELL_OVERLAYS1);
        layerOverlay->ClearObject(mIndicators[indexInd]);

        const GameObjectType blockType = mIndicators[indexInd]->GetBlockType();

        // TODO get conquer time from unit
        constexpr float TIME_BUILD = 2.f;
        mScreen->CreateProgressBar(nextCell, TIME_BUILD, player,
                                   [this, nextCell, player, blockType]
        {
            mGameMap->BuildWall(nextCell, player, blockType);

            ++mNextCell;

            if(mNextCell < mCells.size())
                mState = START_NEXT;
            else
                mState = COMPLETED;
        });

        return ;
    }

    mState = FAILED;
}

void WallBuildPath::UpdatePathCost()
{
    // TODO proper cost computation
    mEnergyCost = (mCells.size() - 1) * 5.f;
    mMaterialCost = (mCells.size() - 1) * 10.f;
}

void WallBuildPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    CreateIndicators();

    // skip first cell as it's unit location
    mNextCell = 1;

    // stat conquering first cell
    InitNextBuild();
}

void WallBuildPath::Update(float delta)
{
    if(START_NEXT == mState)
        InitNextBuild();
}

void WallBuildPath::SetIndicatorsType(const std::vector<Cell2D> & cells,
                                      const std::vector<WallIndicator *> & indicators)
{
    const unsigned int lastIdx = cells.size() - 1;
    const unsigned int lastIndicator = lastIdx - 1;

    if(0 == lastIndicator)
    {
        const int br = cells[1].row - cells[0].row;
        const int bc = cells[1].col - cells[0].col;
        const int ar = 0;
        const int ac = 0;

        indicators[0]->SetBeforeAfterDirections(br, bc, ar, ac);
    }
    else
    {
        const int ar = cells[2].row - cells[1].row;
        const int ac = cells[2].col - cells[1].col;

        indicators[0]->SetBeforeAfterDirections(0, 0, ar, ac);

        // 2nd to n-1 indicators
        for(unsigned int i = 1; i < lastIndicator; ++i)
        {
            const int br = cells[i + 1].row - cells[i].row;
            const int bc = cells[i + 1].col - cells[i].col;

            const int ar = cells[i + 2].row - cells[i + 1].row;
            const int ac = cells[i + 2].col - cells[i + 1].col;

            indicators[i]->SetBeforeAfterDirections(br, bc, ar, ac);
        }

        // set directions for last indicator
        const int br = cells[lastIdx].row - cells[lastIdx - 1].row;
        const int bc = cells[lastIdx].col - cells[lastIdx - 1].col;
        indicators[lastIndicator]->SetBeforeAfterDirections(br, bc, 0, 0);
    }
}

} // namespace game
