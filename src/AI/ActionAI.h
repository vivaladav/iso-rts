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

// ===== ACTION AI =====
struct ActionAI
{
    GameObject * ObjSrc = nullptr;
    GameObject * ObjDst = nullptr;
    Cell2D cellSrc;
    Cell2D cellDst;
    AIActionId aid = AIA_NOP;
    int priority = 0;
};

struct ActionAiComp
{
    bool operator()(const ActionAI * a, const ActionAI * b)
    {
        return a->priority < b->priority;
    }
};

// ===== ACTION AI NEW UNIT =====

struct ActionAINewUnit : public ActionAI
{
    UnitType unitType;
};


} // namespace game
