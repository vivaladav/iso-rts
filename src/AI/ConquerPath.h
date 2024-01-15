#pragma once

#include <functional>
#include <vector>

namespace game
{

class CellProgressBar;
class ConquestIndicator;
class GameMap;
class IsoMap;
class ScreenGame;
class Unit;

class ConquerPath
{
public:
    enum ConquerState : unsigned int
    {
        READY,
        MOVING,
        CONQUERING,
        COMPLETED,
        FAILED,
        ABORTING,
        ABORTED,

        NUM_PATH_STATES
    };

public:
    ConquerPath(Unit * unit, IsoMap * im, GameMap * gm, ScreenGame * sg);
    ~ConquerPath();

    Unit * GetUnit() const;

    ConquerState GetState() const;

    float GetPathCost() const;

    void SetPathCells(const std::vector<unsigned int> & cells);

    void SetOnCompleted(const std::function<void()> & f);
    void SetOnFailed(const std::function<void()> & f);
    void SetOnAborted(const std::function<void()> & f);

    void Start();
    void Abort();
    void InstantAbort();

    void Update(float delta);

    void Finish();

private:
    void CreateIndicators();

    void InitNextConquest();
    void InitNextMove();

    void UpdateMove(float delta);

    void UpdatePathCost();

    void Fail();

private:
    std::vector<unsigned int> mCells;

    std::vector<ConquestIndicator *> mIndicators;

    std::function<void()> mOnCompleted;
    std::function<void()> mOnFailed;
    std::function<void()> mOnAborted;

    CellProgressBar * mProgressBar = nullptr;

    Unit * mUnit = nullptr;

    IsoMap * mIsoMap = nullptr;
    GameMap * mGameMap = nullptr;

    ScreenGame * mScreen = nullptr;

    ConquerState mState = READY;

    unsigned int mNextCell = 0;

    float mCost = 0.f;

    // movement
    float mObjX = 0.f;
    float mObjY = 0.f;
    float mVelX = 0.f;
    float mVelY = 0.f;
    float mTargetX = 0.f;
    float mTargetY = 0.f;

    bool mLocalPlayer = false;
};

inline Unit * ConquerPath::GetUnit() const { return mUnit; }

inline ConquerPath::ConquerState ConquerPath::GetState() const { return mState; }

inline float ConquerPath::GetPathCost() const { return mCost; }

inline void ConquerPath::SetPathCells(const std::vector<unsigned int> & cells)
{
    mCells = cells;

    UpdatePathCost();
}

inline void ConquerPath::SetOnCompleted(const std::function<void()> & f) {  mOnCompleted = f; }
inline void ConquerPath::SetOnFailed(const std::function<void()> & f) { mOnFailed = f; }
inline void ConquerPath::SetOnAborted(const std::function<void()> & f) { mOnAborted = f; }

} // namespace game
