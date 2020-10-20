#include "PlayerAI.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "Player.h"

#include <utilities/LoadedDie.h>

#include <algorithm>
#include <iostream>

namespace game
{

PlayerAI::PlayerAI(Player * player)
    : mPlayer(player)
{

}

void PlayerAI::DecideActions()
{
    const unsigned int rows = mGm->GetNumRows();
    const unsigned int cols = mGm->GetNumCols();

    // -- populate own and enemy cells --
    std::vector<GameMapCell> ownCells;
    std::vector<GameMapCell> enemyCells;

    for(unsigned int r = 0; r < rows; ++r)
    {
        for(unsigned int c = 0; c < cols; ++c)
        {
            const GameMapCell & cell = mGm->GetCell(r, c);

            if(cell.ownerId == mPlayer->GetPlayerId())
                ownCells.push_back(cell);
            else if(cell.ownerId != -1)
                enemyCells.push_back(cell);
        }
    }

    std::cout << "own cells:" << ownCells.size() << std::endl;
    std::cout << "enemy cells:" << enemyCells.size() << std::endl;

    std::vector<int> enemyDist = FindCellDistances(ownCells, enemyCells);

    // -- visit own cells --
    const unsigned int numOwnCells = ownCells.size();

    for(unsigned int c = 0; c < numOwnCells; ++c)
    {
        const Cell2D pos {ownCells[c].row, ownCells[c].col};

        std::vector<AIActionId> actions;

        // init possible actions
        if(mGm->CanFortifyCell(pos, mPlayer))
            actions.emplace_back(ACT_CELL_FORTIFY);
        if(mGm->CanUpgradeCell(pos, mPlayer))
            actions.emplace_back(ACT_CELL_UPGRADE);
        if(mGm->CanCreateUnit(pos, mPlayer))
            actions.emplace_back(ACT_NEW_UNIT);
        if(mGm->CanUpgradeUnit(pos, mPlayer))
            actions.emplace_back(ACT_UNIT_UPGRADE);
        // TODO handle unit move
//        if(ownCells[c].units > 0)
//            actions.emplace_back(ACT_UNIT_MOVE);

        // no action available for now -> skip this cell
        if(actions.empty())
            continue;

        const AIActionId actId = DecideCellAction(ownCells[c], actions, enemyDist[c]);
        std::cout << "ACTION " << actId << std::endl;
        const int actPriority = MakeCellPriority(ownCells[c], enemyDist[c]);

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

std::vector<int> PlayerAI::FindCellDistances(const std::vector<GameMapCell> & ownCells,
                                             const std::vector<GameMapCell> & enemyCells)
{
    std::vector<int> dist;
    dist.reserve(ownCells.size());

    const unsigned int min = mGm->GetNumRows() * mGm->GetNumCols();

    for(const GameMapCell & oCell : ownCells)
    {
        int minDist = min;

        for(const GameMapCell & eCell : enemyCells)
        {
            const int dist = abs(eCell.row - oCell.row) + abs(eCell.col - oCell.col);

            if(dist < minDist)
                minDist = dist;
        }

        dist.emplace_back(minDist);
    }

    return dist;
}

AIActionId PlayerAI::DecideCellAction(const GameMapCell & cell,
                                      const std::vector<AIActionId> & actions,
                                      int enemyDist)
{
    const unsigned int numActions = actions.size();

    // -- find consts and most expensive action --
    int maxCost = 0;

    std::vector<int> costs;
    costs.reserve(numActions);

    for(AIActionId aid : actions)
    {
        int cost = 0;

        switch(aid)
        {
            case ACT_NEW_UNIT:
                cost = COST_NEW_UNIT[cell.units];
            break;

            case ACT_UNIT_UPGRADE:
                cost = COST_UNIT_UPGRADE[cell.unitsLevel];
            break;

            case ACT_CELL_FORTIFY:
                cost = COST_CELL_FORT[cell.fortLevel];
            break;

            case ACT_CELL_UPGRADE:
                cost = COST_CELL_UPGRADE[cell.level];
            break;

            default:
            break;
        }

        if(cost > maxCost)
            maxCost = cost;

        costs.emplace_back(cost);
    }

    //  -- distance from enemy --
    const unsigned int rows = mGm->GetNumRows();
    const unsigned int cols = mGm->GetNumCols();
    const int maxDist = (rows - 1) + (cols - 1);

    const float distScore = 100.f * (maxDist - enemyDist) / maxDist;

    // -- define probability for each action --
    std::vector<float> probs;
    probs.reserve(numActions);

    for(unsigned int i = 0; i < numActions; ++i)
    {
        const AIActionId aid = actions[i];

        float prob = 0.f;

        // add cost factor
        prob += 100.f * (maxCost - costs[i]) / maxCost;

        // add action dependent factors
        switch(aid)
        {
            case ACT_NEW_UNIT:
            {
                prob += distScore;

                prob += 100.f * (MAX_CELL_UNITS - cell.units) / MAX_CELL_UNITS;
            }
            break;

            case ACT_UNIT_UPGRADE:
            {
                prob += distScore * 0.75f;

                prob += 100.f * (MAX_UNITS_LEVEL - cell.unitsLevel) / MAX_UNITS_LEVEL;
            }
            break;

            case ACT_CELL_FORTIFY:
            {
                prob += distScore * 0.75f;

                prob += 100.f * (MAX_CELL_FORT_LEVEL - cell.fortLevel) / MAX_CELL_FORT_LEVEL;
            }
            break;

            case ACT_CELL_UPGRADE:
            {
                // cell upgrade should be more likely when far from enemiesb
                prob += 100.f - distScore;

                prob += 100.f * (MAX_CELL_LEVEL - cell.level) / MAX_CELL_LEVEL;
            }
            break;

            default:
            break;
        }

        probs.emplace_back(prob);

        std::cout << "ACTION " << aid << " - prob: " << prob << std::endl;
    }

    lib::utilities::LoadedDie die(probs);

    return actions[die.GetNextValue()];
}

int PlayerAI::MakeCellPriority(const GameMapCell & cell, int enemyDist) const
{
    int priority = 0;

    // distance from enemy
    const unsigned int rows = mGm->GetNumRows();
    const unsigned int cols = mGm->GetNumCols();
    const int maxDist = (rows - 1) + (cols - 1);

    priority += 100 * (maxDist - enemyDist) / maxDist;

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
