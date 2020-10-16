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
        const Cell2D pos {ownCells[c].row, ownCells[c].col};

        std::vector<AIActionId> actions;

        // init possible actions
        if(gm->CanFortifyCell(pos, mPlayer))
            actions.emplace_back(ACT_CELL_FORTIFY);
        if(gm->CanUpgradeCell(pos, mPlayer))
            actions.emplace_back(ACT_CELL_UPGRADE);
        if(gm->CanCreateUnit(pos, mPlayer))
            actions.emplace_back(ACT_NEW_UNIT);
        if(gm->CanUpgradeUnit(pos, mPlayer))
            actions.emplace_back(ACT_UNIT_UPGRADE);
        // TODO handle unit move
//        if(ownCells[c].units > 0)
//            actions.emplace_back(ACT_UNIT_MOVE);

        // no action available for now -> skip this cell
        if(actions.empty())
            continue;

        const AIActionId actId = DecideCellAction(actions);
        const int actPriority = MakeCellPriority(ownCells[c], enemyCells, gm);

        ActionAI action =
        {
            actId,
            actPriority,
            pos,
            pos
        };

        AddNewAction(action);
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

AIActionId PlayerAI::DecideCellAction(const std::vector<AIActionId> & actions)
{
    // test logic, to replace with proper code
    auto it = std::find(actions.begin(), actions.end(), ACT_NEW_UNIT);

    if(it != actions.end())
        return ACT_NEW_UNIT;
    else
        return ACT_NOP;
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
    const int maxPriorityUnits = 52;
    const int incPriorityUnits = maxPriorityUnits / MAX_CELL_UNITS;

    priority += maxPriorityUnits - (incPriorityUnits * cell.units);

    // add fortification val
    const int maxPriorityFort = 24;
    const int incPriorityFort = maxPriorityFort / MAX_CELL_FORT_LEVEL;

    priority += maxPriorityFort - (incPriorityFort * cell.fortLevel);

    // add cell level val
    const int maxPriorityLevel = 24;
    const int incPriorityLevel = maxPriorityLevel / MAX_CELL_LEVEL;

    priority += maxPriorityLevel - (incPriorityLevel * cell.fortLevel);

    return priority;
}

void PlayerAI::AddNewAction(const ActionAI & action)
{
    // insert action if not already in the queue
    bool found = false;

    for(ActionAI & a : mActions)
    {
        // action already in queue -> update its priority
        if(a == action)
        {
            found = true;

            // update queue
            a.priority = action.priority;
            std::make_heap(mActions.begin(), mActions.end(), ActionAiComp{});

            break;
        }
    }

    // add new action
    if(!found)
    {
        std::cout << "NEW ACTION " << action.aid
                  << " - priority: " << action.priority << std::endl;
        PushAction(action);
    }
}

} // namespace game
