#pragma once

#include "Cell2D.h"

namespace game
{

enum AIActionId : unsigned int
{
    ACT_NOP,

    ACT_NEW_UNIT,

    NUM_AI_ACTIONS
};

struct ActionAI
{
    AIActionId aid;
    int priority;
    Cell2D src;
    Cell2D dst;
};

struct ActionAiComp
{
    bool operator()(const ActionAI & a, const ActionAI & b)
    {
        return a.priority < b.priority;
    }
};

} // namespace game
