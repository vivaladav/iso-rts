#include "PlayerAI.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "Player.h"
#include "GameObjects/Unit.h"

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

            if(cell.owner == mPlayer)
                ownCells.push_back(cell);
            else if(cell.owner != nullptr)
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
        if(mGm->CanCreateUnit(pos, mPlayer))
            actions.emplace_back(ACT_NEW_UNIT);
        if(mGm->CanUpgradeUnit(pos, mPlayer))
            actions.emplace_back(ACT_UNIT_UPGRADE);
        if(CanMoveUnit(ownCells[c]))
            actions.emplace_back(ACT_UNIT_MOVE);
        if(CanDestroyUnit(ownCells[c]))
            actions.emplace_back(ACT_UNIT_DESTROY);

        // no action available for now -> skip this cell
        if(actions.empty())
            continue;

        const AIActionId actId = DecideCellAction(ownCells[c], actions, enemyDist[c]);
        const int cellPriority = MakeCellPriority(ownCells[c], enemyDist[c]);
        std::cout << "ACTION " << actId << " - cell priority:" << cellPriority << std::endl;

        Cell2D dest = pos;

        if(actId == ACT_UNIT_MOVE)
            dest = DecideMoveDestination(ownCells[c]);

        ActionAI action =
        {
            pos,
            dest,
            actId,
            cellPriority
        };

        AddNewAction(action);
    }

    std::cout << "actions:" << mActions.size() << std::endl;
}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return { {0,0}, {0,0}, ACT_NOP, 0 };

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
            {
                const Unit * unit = cell.GetUnit();
                cost = ENERGY_NEW_UNIT;
            }
            break;

            case ACT_UNIT_UPGRADE:
            {
                const Unit * unit = cell.GetUnit();
                cost = unit ? COST_UNIT_UPGRADE[unit->GetUnitLevel()] : 0;
            }
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
            }
            break;

            case ACT_UNIT_UPGRADE:
            {
                prob += distScore * 0.75f;

                const Unit * unit = cell.GetUnit();
                const int unitLevel = unit ? unit->GetUnitLevel() : 0;
                prob += 100.f * (MAX_UNITS_LEVEL - unitLevel) / MAX_UNITS_LEVEL;
            }
            break;

            case ACT_UNIT_DESTROY:
            {
                // unit destroy should be more likely when far from enemies
                prob += 100.f - distScore;

                // more units means higher probability
                prob += 100.f * mPlayer->GetNumUnits() / mPlayer->GetNumCells();
            }

            case ACT_UNIT_MOVE:
            {
                prob += distScore;
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
    const Unit * unit = cell.GetUnit();

    const int maxPriorityUnits = 52;
    const int incPriorityUnits = unit ? maxPriorityUnits : 0;


    priority += incPriorityUnits;

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

bool PlayerAI::CanMoveUnit(const GameMapCell & cell) const
{
    // no units to move
    if(!cell.HasUnit())
        return false;

    const int r0 = cell.row;
    const int c0 = cell.col;

    const int rows = mGm->GetNumRows();
    const int cols = mGm->GetNumCols();

    // check prev row
    if(r0 > 0)
    {
        const int r1 = r0 - 1;

        // TOP LEFT
        if(c0 > 0 && mGm->GetCell(r1, c0 - 1).walkable)
            return true;
        // TOP
        if(mGm->GetCell(r1, c0).walkable)
            return true;
        // TOP RIGHT
        if(c0 < (cols - 1) && mGm->GetCell(r1, c0 + 1).walkable)
            return true;
    }

    // check cell row
    // LEFT
    if(c0 > 0 && mGm->GetCell(r0, c0 - 1).walkable)
        return true;
    // RIGHT
    if(c0 < (cols - 1) && mGm->GetCell(r0, c0 + 1).walkable)
        return true;

    // check next row
    if(r0 < (rows - 1))
    {
        const int r2 = r0 + 1;

        // BOTTOM LEFT
        if(c0 > 0 && mGm->GetCell(r2, c0 - 1).walkable)
            return true;
        // BOTTOM
        if(mGm->GetCell(r2, c0).walkable)
            return true;
        // BOTTOM RIGHT
        if(c0 < (cols - 1) && mGm->GetCell(r2, c0 + 1).walkable)
            return true;
    }

    // no walkable cell found
    return false;
}

bool PlayerAI::CanDestroyUnit(const GameMapCell & cell) const
{
    // true if cell has units and player has no energy
    return cell.HasUnit() && mPlayer->GetEnergy() < 0;
}

Cell2D PlayerAI::DecideMoveDestination(const GameMapCell & cell) const
{
    const int r0 = cell.row;
    const int c0 = cell.col;

    const int rows = mGm->GetNumRows();
    const int cols = mGm->GetNumCols();

    std::vector<Cell2D> dest;
    std::vector<float> probs;

    const float weightUnits = 100.f;
    const float weightFree = 100.f;

    // check prev row
    if(r0 > 0)
    {
        const int r1 = r0 - 1;

        // TOP LEFT
        if(c0 > 0)
        {
            const GameMapCell & tl = mGm->GetCell(r1, c0 - 1);

            if(tl.walkable)
            {
                dest.emplace_back(r1, c0 - 1);

                const Unit * unit = tl.GetUnit();
                const float prob = unit ? weightUnits : 0.f;
                const float prob2 = tl.owner == nullptr ? weightFree : 0.f;
                probs.emplace_back(prob + prob2);
            }
        }

        // TOP
        const GameMapCell & tc = mGm->GetCell(r1, c0);

        if(tc.walkable)
        {
            dest.emplace_back(r1, c0);

            const Unit * unit = tc.GetUnit();
            const float prob = unit ? weightUnits : 0.f;
            const float prob2 = tc.owner == nullptr ? weightFree : 0.f;
            probs.emplace_back(prob + prob2);
        }

        // TOP RIGHT
        if(c0 < (cols - 1))
        {
            const GameMapCell & tr = mGm->GetCell(r1, c0 + 1);

            if(tr.walkable)
            {
                dest.emplace_back(r1, c0 + 1);

                const Unit * unit = tr.GetUnit();
                const float prob = unit ? weightUnits : 0.f;
                const float prob2 = tr.owner == nullptr ? weightFree : 0.f;
                probs.emplace_back(prob + prob2);

            }
        }
    }

    // check cell row
    // LEFT
    if(c0 > 0)
    {
        const GameMapCell & l = mGm->GetCell(r0, c0 - 1);

        if(l.walkable)
        {
            dest.emplace_back(r0, c0 - 1);

            const Unit * unit = l.GetUnit();
            const float prob = unit ? weightUnits : 0.f;
            const float prob2 = l.owner == nullptr ? weightFree : 0.f;
            probs.emplace_back(prob + prob2);
        }
    }

    // RIGHT
    if(c0 < (cols - 1))
    {
        const GameMapCell & r = mGm->GetCell(r0, c0 + 1);

        if(r.walkable)
        {
            dest.emplace_back(r0, c0 + 1);

            const Unit * unit = r.GetUnit();
            const float prob = unit ? weightUnits : 0.f;
            const float prob2 = r.owner == nullptr ? weightFree : 0.f;
            probs.emplace_back(prob + prob2);
        }
    }

    // check next row
    if(r0 < (rows - 1))
    {
        const int r2 = r0 + 1;

        // BOTTOM LEFT
        if(c0 > 0)
        {
            const GameMapCell & bl = mGm->GetCell(r2, c0 - 1);

            if(bl.walkable)
            {
                dest.emplace_back(r2, c0 - 1);

                const Unit * unit = bl.GetUnit();
                const float prob = unit ? weightUnits : 0.f;
                const float prob2 = bl.owner == nullptr ? weightFree : 0.f;
                probs.emplace_back(prob + prob2);
            }
        }

        // BOTTOM
        const GameMapCell & bc = mGm->GetCell(r2, c0);

        if(bc.walkable)
        {
            dest.emplace_back(r2, c0);

            const Unit * unit = bc.GetUnit();
            const float prob = unit ? weightUnits : 0.f;
            const float prob2 = bc.owner == nullptr ? weightFree : 0.f;
            probs.emplace_back(prob + prob2);
        }

        // BOTTOM RIGHT
        if(c0 < (cols - 1))
        {
            const GameMapCell & br = mGm->GetCell(r2, c0 + 1);

            if(br.walkable)
            {
                dest.emplace_back(r2, c0 + 1);

                const Unit * unit = br.GetUnit();
                const float prob = unit ? weightUnits : 0.f;
                const float prob2 = br.owner == nullptr ? weightFree : 0.f;
                probs.emplace_back(prob + prob2);
            }
        }
    }

    lib::utilities::LoadedDie die(probs);

    return dest[die.GetNextValue()];
}

} // namespace game
