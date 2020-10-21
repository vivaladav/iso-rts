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

    void SetGameMap(GameMap * gm);

    void DecideActions();

    ActionAI GetNextAction();

    Player * GetPlayer();

private:
    void PushAction(const ActionAI & action);
    ActionAI PopAction();

    std::vector<int> FindCellDistances(const std::vector<GameMapCell> & ownCells,
                                       const std::vector<GameMapCell> & enemyCells);

    AIActionId DecideCellAction(const GameMapCell & cell,
                                const std::vector<AIActionId> & actions,
                                int enemyDist);

    int MakeCellPriority(const GameMapCell & cell,
                         int enemyDist) const;

    void AddNewAction(const ActionAI & action);

    bool CanCellMove(const GameMapCell & cell) const;
    Cell2D DecideMoveDestination(const GameMapCell & cell) const;
    int DecideNumUnitsToMove(const GameMapCell & cell) const;

private:
    std::vector<ActionAI> mActions;

    Player * mPlayer = nullptr;

    GameMap * mGm = nullptr;
};

inline void PlayerAI::SetGameMap(GameMap * gm) { mGm = gm; }

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
