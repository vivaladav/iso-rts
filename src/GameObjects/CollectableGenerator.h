#pragma once

namespace game
{

class GameMap;

class CollectableGenerator
{
public:
    CollectableGenerator(GameMap * gm, int timeMin, int timeMax);
    virtual ~CollectableGenerator();

    void SetCell(int row, int col);
    int GetRow() const;
    int GetCol() const;

    void ResetTimer();

    void Update(float delta);

protected:
    GameMap * GetGameMap() const;

private:
    virtual void OnGeneration() = 0;

private:
    GameMap * mGameMap = nullptr;

    int mTimeMin = 0;
    int mTimeMax = 0;

    float mTimeRegen = 0.f;
    float mTimerRegen = 0.f;

    int mRow = -1;
    int mCol = -1;
};

inline void CollectableGenerator::SetCell(int row, int col)
{
    mRow = row;
    mCol = col;
}

inline int CollectableGenerator::GetRow() const { return mRow; }

inline int CollectableGenerator::GetCol() const { return mCol; }


inline GameMap * CollectableGenerator::GetGameMap() const
{
    return mGameMap;
}

} // namespace game
