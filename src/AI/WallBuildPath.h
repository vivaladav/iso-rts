#pragma once

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
        COMPLETED,
        FAILED,

        NUM_PATH_STATES
    };

public:
    WallBuildPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg);
    ~WallBuildPath();

    GameObject * GetObject() const;

    BuildState GetState() const;

    float GetEnergyCost() const;
    float GetMateriaCost() const;

    void SetPathCells(const std::vector<unsigned int> & cells);

    void Start();

    void Update(float delta);

private:
    void CreateIndicators();

    void InitNextBuild();

    void UpdatePathCost();

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

    float mEnergyCost = 0.f;
    float mMaterialCost = 0.f;
};

inline WallBuildPath::WallBuildPath(GameObject * obj, IsoMap * im, GameMap * gm, ScreenGame * sg)
    : mObj(obj), mIsoMap(im), mGameMap(gm), mScreen(sg)
{
}

inline GameObject * WallBuildPath::GetObject() const { return mObj; }

inline WallBuildPath::BuildState WallBuildPath::GetState() const { return mState; }

inline float WallBuildPath::GetEnergyCost() const { return mEnergyCost; }
inline float WallBuildPath::GetMateriaCost() const { return mMaterialCost; }

inline void WallBuildPath::SetPathCells(const std::vector<unsigned int> & cells)
{
    mCells = cells;

    UpdatePathCost();
}

} // namespace game
