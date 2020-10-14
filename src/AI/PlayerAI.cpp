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
    std::vector<Cell2D> ownCellsPos;
    std::vector<GameMapCell> enemyCells;
    std::vector<Cell2D> enemyCellsPos;

    for(unsigned int r = 0; r < rows; ++r)
    {
        for(unsigned int c = 0; c < cols; ++c)
        {
            const GameMapCell & cell = gm->GetCell(r, c);

            if(cell.ownerId == mPlayer->GetPlayerId())
            {
                ownCells.push_back(cell);
                ownCellsPos.emplace_back(r, c);
            }
            else if(cell.ownerId != -1)
            {
                enemyCells.push_back(cell);
                enemyCellsPos.emplace_back(r, c);
            }
        }
    }

    std::cout << "own cells:" << ownCells.size() << std::endl;
    std::cout << "enemy cells:" << enemyCells.size() << std::endl;

    // -- visit own cells --
    const unsigned int numOwnCells = ownCells.size();

    for(unsigned int c = 0; c < numOwnCells; ++c)
    {
        // TEST
        if(gm->CanCreateUnit(ownCellsPos[c], mPlayer))
        {
            const ActionAI action =
            {
                ACT_NEW_UNIT,
                1,
                ownCellsPos[c],
                ownCellsPos[c]
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

} // namespace game
