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

    int MakeCellPriority(const GameMapCell & cell,
                         const std::vector<GameMapCell> & enemyCells,
                         const GameMap * gm) const;

private:
    std::vector<ActionAI> mActions;

    Player * mPlayer = nullptr;
};

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
