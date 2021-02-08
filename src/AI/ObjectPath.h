#pragma once

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

    void SetPathCells(const std::vector<unsigned int> & cells);

    void Start();

    void Update(float delta);

private:
    void InitNextMoveStep();

private:
    std::vector<unsigned int> mCells;

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

inline void ObjectPath::SetPathCells(const std::vector<unsigned int> & cells) { mCells = cells; }

inline void ObjectPath::Start()
{
    // do nothing if already started
    if(mState != READY)
        return ;

    mState = RUNNING;

    InitNextMoveStep();
}

} // namespace game
