#pragma once

#include <functional>
#include <vector>

namespace game
{

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
        CONQUERING_CELL,
        CONQUER_NEXT,
        COMPLETED,
        FAILED,
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

    void Start();
    void Abort();

    void Update(float delta);

private:
    void CreateIndicators();

    void InitNextConquest();

    void UpdatePathCost();

    void FinishAbortion();

    void Fail();

private:
    std::vector<unsigned int> mCells;

    std::vector<ConquestIndicator *> mIndicators;

    std::function<void()> mOnCompleted;
    std::function<void()> mOnFailed;

    Unit * mUnit = nullptr;

    IsoMap * mIsoMap = nullptr;
    GameMap * mGameMap = nullptr;

    ScreenGame * mScreen = nullptr;

    ConquerState mState = READY;

    unsigned int mNextCell = 0;

    float mCost = 0.f;
};

inline ConquerPath::ConquerPath(Unit * unit, IsoMap * im, GameMap * gm, ScreenGame * sg)
    : mOnCompleted([]{}), mOnFailed([]{}), mUnit(unit), mIsoMap(im), mGameMap(gm), mScreen(sg)
{
}

inline Unit * ConquerPath::GetUnit() const { return mUnit; }

inline ConquerPath::ConquerState ConquerPath::GetState() const { return mState; }

inline float ConquerPath::GetPathCost() const { return mCost; }

inline void ConquerPath::SetPathCells(const std::vector<unsigned int> & cells)
{
    mCells = cells;

    UpdatePathCost();
}

inline void ConquerPath::SetOnCompleted(const std::function<void()> & f)
{
    mOnCompleted = f;
}

inline void ConquerPath::SetOnFailed(const std::function<void()> & f)
{
    mOnFailed = f;
}

} // namespace game
