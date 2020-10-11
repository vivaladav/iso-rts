#pragma once

#include "ActionAI.h"

#include <queue>
#include <vector>

namespace game
{

class GameMap;
class Player;

class PlayerAI
{
public:
    PlayerAI(Player * player);

    void DecideActions(GameMap * gm);

    ActionAI GetNextAction();

    Player * GetPlayer();

private:
    std::priority_queue<ActionAI, std::vector<ActionAI>, ActionAiComp> mActions;

    Player * mPlayer = nullptr;
};

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
