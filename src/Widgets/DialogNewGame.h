#pragma once

#include <sgl/sgui/Widget.h>


namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui
    {
        class AbstractButton;
        class AbstractButtonsGroup;
        class Image;
    }
}

namespace game
{

class Game;

class DialogNewGame : public sgl::sgui::Widget
{
public:
    DialogNewGame(Game * game);
    ~DialogNewGame();

private:
    void CreatePanelMode();
    void CreatePanelDifficulty();
    void CreatePanelSettings();

    void HandlePositionChanged() override;

    void SetPositions();

private:
    sgl::graphic::Image * mBgL = nullptr;
    sgl::graphic::Image * mBgC = nullptr;
    sgl::graphic::Image * mBgR = nullptr;

    sgl::sgui::Image * mPanelMode = nullptr;
    sgl::sgui::Image * mPanelDiff = nullptr;
    sgl::sgui::Image * mPanelSett = nullptr;

    sgl::sgui::AbstractButton * mButtonClose = nullptr;
    sgl::sgui::AbstractButton * mButtonContinue = nullptr;

    sgl::sgui::AbstractButtonsGroup * mButtonsMode = nullptr;
    sgl::sgui::AbstractButtonsGroup * mButtonsDiff = nullptr;
    sgl::sgui::AbstractButtonsGroup * mButtonsSett = nullptr;

    Game * mGame = nullptr;
};

} // namespace game
