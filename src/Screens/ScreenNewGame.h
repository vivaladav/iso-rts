#pragma once

#include "Screen.h"

#include <vector>

namespace lib
{
    namespace sgui
    {
        class Label;
        class PushButton;
    }
}

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
    MapPreview * GetMapPreview(unsigned int index);
    void ShowMapPreviews(unsigned int mapStart);

    void UpdateNavButtonsState();

private:
    std::vector<MapPreview *> mMapPreviews;
    int mMapSelInd = -1;
    unsigned int mStartMap = 0;

    lib::sgui::Label * mLabelMaps = nullptr;

    lib::sgui::PushButton * mButtonStart = nullptr;

    lib::sgui::PushButton * mButtonNavL = nullptr;
    lib::sgui::PushButton * mButtonNavR = nullptr;

    Game * mGame = nullptr;

    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
