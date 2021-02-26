#pragma once

namespace game
{

class GameMap;

class DiamondsGenerator
{
public:
    DiamondsGenerator(GameMap * gm);
    void SetCell(int row, int col);

    void Update(float delta);

private:
    GameMap * mGameMap = nullptr;

    float mTimeRegen = 0.f;
    float mTimerRegen = 0.f;

    int mRow = -1;
    int mCol = -1;
};

inline void DiamondsGenerator::SetCell(int row, int col)
{
    mRow = row;
    mCol = col;
}

} // namespace game
