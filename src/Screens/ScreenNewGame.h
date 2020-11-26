#pragma once

#include "Screen.h"

#include <vector>

namespace lib { namespace sgui { class PushButton; } }

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

    lib::sgui::PushButton * mButtonStart = nullptr;

    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
