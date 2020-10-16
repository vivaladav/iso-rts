#include "PlayerAI.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "Player.h"

#include <algorithm>
#include <iostream>

namespace game
{

PlayerAI::PlayerAI(Player * player)
    : mPlayer(player)
{

}

void PlayerAI::DecideActions(GameMap * gm)
{
    const unsigned int rows = gm->GetNumRows();
    const unsigned int cols = gm->GetNumCols();

    // -- populate own and enemy cells --
    std::vector<GameMapCell> ownCells;
    std::vector<GameMapCell> enemyCells;

    for(unsigned int r = 0; r < rows; ++r)
    {
        for(unsigned int c = 0; c < cols; ++c)
        {
            const GameMapCell & cell = gm->GetCell(r, c);

            if(cell.ownerId == mPlayer->GetPlayerId())
                ownCells.push_back(cell);
            else if(cell.ownerId != -1)
                enemyCells.push_back(cell);
        }
    }

    std::cout << "own cells:" << ownCells.size() << std::endl;
    std::cout << "enemy cells:" << enemyCells.size() << std::endl;

    // -- visit own cells --
    const unsigned int numOwnCells = ownCells.size();

    for(unsigned int c = 0; c < numOwnCells; ++c)
    {
        Cell2D pos {ownCells[c].row, ownCells[c].col};

        // TEST
        if(gm->CanCreateUnit(pos, mPlayer))
        {
            const int actPriority = MakeCellPriority(ownCells[c], enemyCells, gm);

            const ActionAI action =
            {
                ACT_NEW_UNIT,
                actPriority,
                pos,
                pos
            };

            // insert action if not already in the queue
            bool found = false;

            for(ActionAI & a : mActions)
            {
                if(a == action)
                {
                    found = true;

                    // update queue
                    a.priority = action.priority;
                    std::make_heap(mActions.begin(), mActions.end(), ActionAiComp{});

                    break;
                }
            }

            if(!found)
            {
                std::cout << "NEW ACTION " << action.aid
                          << " - priority: " << action.priority << std::endl;
                PushAction(action);
            }
        }
    }

    std::cout << "actions:" << mActions.size() << std::endl;
}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return { ACT_NOP, 0, {0,0}, {0,0} };

    // return top action
    return PopAction();
}

void PlayerAI::PushAction(const ActionAI & action)
{
    mActions.emplace_back(action);
    std::push_heap(mActions.begin(), mActions.end(), ActionAiComp{});
}

ActionAI PlayerAI::PopAction()
{
    std::pop_heap(mActions.begin(), mActions.end(), ActionAiComp{});

    const ActionAI elem = mActions.back();
    mActions.pop_back();

    return elem;
}

int PlayerAI::MakeCellPriority(const GameMapCell & cell,
                               const std::vector<GameMapCell> & enemyCells,
                               const GameMap * gm) const
{
    int priority = 0;

    // distance from enemy
    const unsigned int numEnemyCells = enemyCells.size();

    const unsigned int rows = gm->GetNumRows();
    const unsigned int cols = gm->GetNumCols();
    const int maxDist = (rows - 1) + (cols - 1);

    int minDist = rows * cols;

    for(unsigned e = 0; e < numEnemyCells; ++e)
    {
        const int dist = abs(enemyCells[e].row - cell.row) +
                         abs(enemyCells[e].col - cell.col);

        if(dist < minDist)
            minDist = dist;
    }

    priority += 100 * (maxDist - minDist) / maxDist;

    // add units val
    const int maxPriorityUnits = 10;
    const int incPriorityUnits = maxPriorityUnits / MAX_CELL_UNITS;

    priority += maxPriorityUnits - (incPriorityUnits * cell.units);

    // add fortification val
    const int maxPriorityFort = 8;
    const int incPriorityFort = maxPriorityFort / MAX_CELL_FORT_LEVEL;

    priority += maxPriorityFort - (incPriorityFort * cell.fortLevel);

    // add cell level val
    const int maxPriorityLevel = 8;
    const int incPriorityLevel = maxPriorityLevel / MAX_CELL_LEVEL;

    priority += maxPriorityLevel - (incPriorityLevel * cell.fortLevel);

    return priority;
}

} // namespace game
