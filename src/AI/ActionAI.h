#pragma once

#include "Cell2D.h"

namespace game
{

class GameObject;

enum UnitType : unsigned int;

enum AIActionId : unsigned int
{
    AIA_NOP,

    AIA_NEW_UNIT,
    AIA_UNIT_MOVE,

    NUM_AI_ACTIONS
};

struct ActionAI
{
    GameObject * ObjSrc = nullptr;
    GameObject * ObjDst = nullptr;
    Cell2D cellSrc;
    Cell2D cellDst;
    AIActionId aid = AIA_NOP;
    int priority = 0;

    friend bool operator==(const ActionAI & a1, const ActionAI & a2);
};

struct ActionAINewUnit : public ActionAI
{
    UnitType unitType;
};

inline bool operator==(const ActionAI & a1, const ActionAI & a2)
{
    return a1.aid == a2.aid &&
           a1.ObjSrc == a2.ObjSrc &&
           a1.ObjDst == a2.ObjDst &&
           a1.cellSrc == a2.cellSrc &&
           a1.cellDst == a2.cellDst;
}

struct ActionAiComp
{
    bool operator()(const ActionAI * a, const ActionAI * b)
    {
        return a->priority < b->priority;
    }
};

} // namespace game
