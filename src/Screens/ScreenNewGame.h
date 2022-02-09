#pragma once

#include "Screen.h"

#include <vector>

namespace sgl
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

    sgl::sgui::Label * mLabelMaps = nullptr;

    sgl::sgui::PushButton * mButtonStart = nullptr;

    sgl::sgui::PushButton * mButtonNavL = nullptr;
    sgl::sgui::PushButton * mButtonNavR = nullptr;

    Game * mGame = nullptr;

    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
