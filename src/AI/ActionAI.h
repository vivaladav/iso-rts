#pragma once

#include "Cell2D.h"

namespace game
{

enum AIActionId : unsigned int
{
    ACT_NOP,

    ACT_NEW_UNIT,
    ACT_UNIT_MOVE,
    ACT_UNIT_UPGRADE,

    NUM_AI_ACTIONS
};

struct ActionAI
{
    Cell2D src;
    Cell2D dst;
    AIActionId aid;
    int priority;

    friend bool operator==(const ActionAI & a1, const ActionAI & a2);
};

inline bool operator==(const ActionAI & a1, const ActionAI & a2)
{
    return a1.aid == a2.aid &&
           a1.src == a2.src &&
           a1.dst == a2.dst;
}

struct ActionAiComp
{
    bool operator()(const ActionAI & a, const ActionAI & b)
    {
        return a.priority < b.priority;
    }
};

} // namespace game
