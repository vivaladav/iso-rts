#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class ImageButton; }
}

namespace game
{

class Game;

class DialogCredits : public sgl::sgui::Widget
{
public:
    DialogCredits(Game * game);

    void AddOnCloseClickedFunction(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

    int GetMouseButtonIndex(int buttonId) const;

    void SetPositions();

private:
    Game * mGame = nullptr;

    sgl::graphic::Image * mBgL = nullptr;
    sgl::graphic::Image * mBgC = nullptr;
    sgl::graphic::Image * mBgR = nullptr;

    sgl::sgui::ImageButton * mButtonClose = nullptr;
};

} // namespace game
