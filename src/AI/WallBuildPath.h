#pragma once

#include "Cell2D.h"

#include <functional>
#include <vector>

namespace game
{

class GameMap;
class GameMapProgressBar;
class IsoMap;
class ScreenGame;
class Unit;
class WallIndicator;

class WallBuildPath
{
public:
    enum BuildState : unsigned int
    {
        READY,
        BUILDING,
        MOVING,
        START_NEXT,
        COMPLETED,
        FAILED,
        ABORTING,
        ABORTED,

        NUM_PATH_STATES
    };

public:
    WallBuildPath(Unit * unit, IsoMap * im, GameMap * gm, ScreenGame * sg);
    ~WallBuildPath();

    Unit * GetUnit() const;

    BuildState GetState() const;

    unsigned int GetEnergyCost() const;
    unsigned int GetMateriaCost() const;

    void SetPathCells(const std::vector<unsigned int> & cells);

    unsigned int GetWallLevel() const;
    void SetWallLevel(unsigned int level);

    void Start();

    void Abort();
    void InstantAbort();

    void SetOnCompleted(const std::function<void()> & f);
    void SetOnFailed(const std::function<void()> & f);
    void SetOnAborted(const std::function<void()> & f);

    void Update(float delta);

    void Finish();

    void SetIndicatorsType(const std::vector<Cell2D> & cells,
                           const std::vector<WallIndicator *> & indicators);

private:
    void CreateIndicators();

    void InitNextBuild();
    void InitNextMove();

    void UpdateMove(float delta);

    void UpdatePathCost();

    void Fail();

private:
    std::vector<unsigned int> mCells;

    std::vector<WallIndicator *> mIndicators;

    std::function<void()> mOnCompleted;
    std::function<void()> mOnFailed;
    std::function<void()> mOnAborted;

    GameMapProgressBar * mProgressBar = nullptr;

    Unit * mUnit = nullptr;

    IsoMap * mIsoMap = nullptr;
    GameMap * mGameMap = nullptr;

    ScreenGame * mScreen = nullptr;

    BuildState mState = READY;

    unsigned int mNextCell = 0;

    float mObjX = 0.f;
    float mObjY = 0.f;

    float mVelX = 0.f;
    float mVelY = 0.f;

    float mTargetX = 0.f;
    float mTargetY = 0.f;
    unsigned int mTargetRow = 0;
    unsigned int mTargetCol = 0;

    unsigned int mEnergyCost = 0.f;
    unsigned int mMaterialCost = 0.f;

    unsigned int mLevel = 0;
};

inline WallBuildPath::WallBuildPath(Unit *unit, IsoMap * im, GameMap * gm, ScreenGame * sg)
    : mOnCompleted([]{})
    , mOnFailed([]{})
    , mOnAborted([]{})
    , mUnit(unit)
    , mIsoMap(im)
    , mGameMap(gm)
    , mScreen(sg)
{
}

inline Unit * WallBuildPath::GetUnit() const { return mUnit; }

inline WallBuildPath::BuildState WallBuildPath::GetState() const { return mState; }

inline unsigned int WallBuildPath::GetEnergyCost() const { return mEnergyCost; }
inline unsigned int WallBuildPath::GetMateriaCost() const { return mMaterialCost; }

inline void WallBuildPath::SetPathCells(const std::vector<unsigned int> & cells)
{
    mCells = cells;

    UpdatePathCost();
}

inline unsigned int WallBuildPath::GetWallLevel() const { return mLevel; }
inline void WallBuildPath::SetWallLevel(unsigned int level) { mLevel = level; }

inline void WallBuildPath::SetOnCompleted(const std::function<void()> & f) { mOnCompleted = f; }
inline void WallBuildPath::SetOnFailed(const std::function<void()> & f) { mOnFailed = f; }
inline void WallBuildPath::SetOnAborted(const std::function<void()> & f) { mOnAborted = f; }

} // namespace game
