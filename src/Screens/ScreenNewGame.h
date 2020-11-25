#pragma once

#include "Screen.h"

#include <vector>

namespace game
{

class MapPreview;

enum Difficulty : unsigned int;

class ScreenNewGame : public Screen
{
public:
    ScreenNewGame(Game * game);
    ~ScreenNewGame();

    void Update(float update) override;
    void Render() override;

private:
    std::vector<MapPreview *> mMapPreviews;
    int mMapSelInd = -1;
    unsigned int mStartMap = 0;

    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
