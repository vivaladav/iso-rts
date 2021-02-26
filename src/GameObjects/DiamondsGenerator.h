#pragma once

namespace game
{

class GameMap;

class DiamondsGenerator
{
public:
    DiamondsGenerator(GameMap * gm);

    void Update(float delta);

private:
    GameMap * mGameMap = nullptr;

    float mTimeRegen = 0.f;
    float mTimerRegen = 0.f;
};

} // namespace game
