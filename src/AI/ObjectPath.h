#pragma once

#include "Cell2D.h"

#include <vector>

namespace game
{

class GameMap;
class GameObject;
class IsoMap;

enum PathState : unsigned int
{
    READY,
    RUNNING,
    COMPLETED,
    FAILED,

    NUM_PATH_STATES
};

class ObjectPath
{
public:
    ObjectPath(GameObject * obj, IsoMap * im, GameMap * gm);

    GameObject * GetObject() const;

    PathState GetState() const;

    void PushCell(const Cell2D & cell);
    void SetCells(const std::vector<Cell2D> & cells);

    void Start();

    void Update(float delta);

private:
    void InitNextMoveStep();

private:
    std::vector<Cell2D> mCells;

    GameObject * mObj = nullptr;

    IsoMap * mIsoMap = nullptr;
    GameMap * mGameMap = nullptr;

    PathState mState = READY;

    unsigned int mNextCell = 0;

    float mObjX = 0.f;
    float mObjY = 0.f;

    float mVelX = 0.f;
    float mVelY = 0.f;

    float mTargetX = 0.f;
    float mTargetY = 0.f;
};

inline ObjectPath::ObjectPath(GameObject * obj, IsoMap * im, GameMap * gm)
    : mObj(obj), mIsoMap(im), mGameMap(gm)
{
}

inline GameObject * ObjectPath::GetObject() const { return mObj; }

inline PathState ObjectPath::GetState() const { return mState; }

inline void ObjectPath::PushCell(const Cell2D & cell) { mCells.emplace_back(cell); }
inline void ObjectPath::SetCells(const std::vector<Cell2D> & cells) { mCells = cells; }

inline void ObjectPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    mState = RUNNING;

    InitNextMoveStep();
}

} // namespace game
