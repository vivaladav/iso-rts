#include "PlayerAI.h"

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
            const int actPriority = MakeCellPriority(ownCells[c], enemyCells);

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
                PushAction(action);
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

int PlayerAI::MakeCellPriority(const GameMapCell & cell, const std::vector<GameMapCell> & enemyCells) const
{
    const unsigned int numEnemyCells = enemyCells.size();

    for(unsigned e = 0; e < numEnemyCells; ++e)
    {
        // TODO find min Manhattan distance to enemy
    }

    return 0;
}

} // namespace game
