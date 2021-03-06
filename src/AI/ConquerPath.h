#pragma once

#include <vector>

namespace game
{

class ConquestIndicator;
class GameMap;
class GameObject;
class IsoMap;
class ScreenGame;

class ConquerPath
{
public:
    enum ConquerState : unsigned int
    {
        READY,
        CONQUER_CELLING,
        MOVING,
        COMPLETED,
        FAILED,
        ABORTING,
        ABORTED,

        NUM_PATH_STATES
    };

public:
    ConquerPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg);
    ~ConquerPath();

    GameObject * GetObject() const;

    ConquerState GetState() const;

    float GetPathCost() const;

    void SetPathCells(const std::vector<unsigned int> & cells);

    void Start();

    void Abort();

    void Update(float delta);

private:
    void CreateIndicators();

    void InitNextConquest();
    void TransitionToMoveStep();
    void InitNextMoveStep();

    void UpdatePathCost();

    void FinishAbortion();

private:
    std::vector<unsigned int> mCells;

    std::vector<ConquestIndicator *> mConquestIndicators;

    GameObject * mObj = nullptr;

    IsoMap * mIsoMap = nullptr;
    GameMap * mGameMap = nullptr;

    ScreenGame * mScreen = nullptr;

    ConquerState mState = READY;

    unsigned int mNextCell = 0;

    float mObjX = 0.f;
    float mObjY = 0.f;

    float mVelX = 0.f;
    float mVelY = 0.f;

    float mTargetX = 0.f;
    float mTargetY = 0.f;

    float mCost = 0.f;
};

inline ConquerPath::ConquerPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg)
    : mObj(obj), mIsoMap(im), mGameMap(gm), mScreen(sg)
{
}

inline GameObject * ConquerPath::GetObject() const { return mObj; }

inline ConquerPath::ConquerState ConquerPath::GetState() const { return mState; }

inline float ConquerPath::GetPathCost() const { return mCost; }

inline void ConquerPath::SetPathCells(const std::vector<unsigned int> & cells)
{
    mCells = cells;

    UpdatePathCost();
}

} // namespace game
