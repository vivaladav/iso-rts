#pragma once

#include "Cell2D.h"

#include <vector>

namespace game
{

class WallIndicator;
class GameMap;
class GameObject;
class IsoMap;
class ScreenGame;

class WallBuildPath
{
public:
    enum BuildState : unsigned int
    {
        READY,
        BUILDING,
        START_NEXT,
        COMPLETED,
        FAILED,
        ABORTED,

        NUM_PATH_STATES
    };

public:
    WallBuildPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg);
    ~WallBuildPath();

    GameObject * GetObject() const;

    BuildState GetState() const;

    unsigned int GetEnergyCost() const;
    unsigned int GetMateriaCost() const;

    void SetPathCells(const std::vector<unsigned int> & cells);

    unsigned int GetWallLevel() const;
    void SetWallLevel(unsigned int level);

    void Start();

    void Abort();

    void Update(float delta);

    void SetIndicatorsType(const std::vector<Cell2D> & cells,
                           const std::vector<WallIndicator *> & indicators);

private:
    void CreateIndicators();

    void InitNextBuild();

    void UpdatePathCost();

    void FinishAbortion();

private:
    std::vector<unsigned int> mCells;

    std::vector<WallIndicator *> mIndicators;

    GameObject * mObj = nullptr;

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

    unsigned int mEnergyCost = 0.f;
    unsigned int mMaterialCost = 0.f;

    unsigned int mLevel = 0;
};

inline WallBuildPath::WallBuildPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg)
    : mObj(obj), mIsoMap(im), mGameMap(gm), mScreen(sg)
{
}

inline GameObject * WallBuildPath::GetObject() const { return mObj; }

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

} // namespace game
