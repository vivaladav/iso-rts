#pragma once

#include <sgl/sgui/Widget.h>


namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui
    {
        class AbstractButton;
        class AbstractButtonsGroup;
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
    void HandlePositionChanged() override;

    void SetPositions();

private:
    sgl::graphic::Image * mBgL = nullptr;
    sgl::graphic::Image * mBgC = nullptr;
    sgl::graphic::Image * mBgR = nullptr;

    sgl::sgui::AbstractButton * mButtonClose = nullptr;
    sgl::sgui::AbstractButton * mButtonContinue = nullptr;

    sgl::sgui::AbstractButtonsGroup * mButtonsFact = nullptr;

    Game * mGame = nullptr;
};

} // namespace game
