#pragma once

#include "ActionAI.h"

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
    void PushAction(const ActionAI & action);
    ActionAI PopAction();

private:
    std::vector<ActionAI> mActions;

    Player * mPlayer = nullptr;
};

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
