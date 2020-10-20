#pragma once

#include "ActionAI.h"

#include <vector>

namespace game
{

class GameMap;
class Player;

struct GameMapCell;

class PlayerAI
{
public:
    PlayerAI(Player * player);

    void DecideActions(GameMap * gm);

    ActionAI GetNextAction();

    Player * GetPlayer();

private:
    void PushAction(const ActionAI & action);
    ActionAI PopAction();

    std::vector<int> FindCellDistances(const std::vector<GameMapCell> & ownCells,
                                       const std::vector<GameMapCell> & enemyCells,
                                       const GameMap * gm);

    AIActionId DecideCellAction(const GameMapCell & cell,
                                const std::vector<AIActionId> & actions,
                                int enemyDist);

    int MakeCellPriority(const GameMapCell & cell,
                         const GameMap * gm,
                         int enemyDist) const;

    void AddNewAction(const ActionAI & action);

private:
    std::vector<ActionAI> mActions;

    Player * mPlayer = nullptr;
};

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
