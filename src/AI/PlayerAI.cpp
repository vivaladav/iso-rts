#include "PlayerAI.h"

#include "GameMap.h"
#include "Player.h"

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
       if(mActions.empty() && 0 == ownCells[c].units && gm->CanCreateUnit(ownCellsPos[c], mPlayer))
       {
           const ActionAI action =
           {
               ACT_NEW_UNIT,
               1,
               ownCellsPos[c],
               ownCellsPos[c]
           };

           mActions.push(action);
       }
    }

    std::cout << "actions:" << mActions.size() << std::endl;
}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return { ACT_NOP, 0, {0,0}, {0,0} };

    // return top action after removing it from the queue
    ActionAI action = mActions.top();
    mActions.pop();

    return action;
}

} // namespace game
