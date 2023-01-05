#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class AbstractButton; }
}

namespace game
{

class Game;

class DialogExit : public sgl::sgui::Widget
{
public:
    DialogExit(Game * game);

    void SetFunctionOnClose(const std::function<void()> & f);

protected:
    void HandlePositionChanged() override;

private:
    void SetPositions();

private:
    sgl::graphic::Image * mBg = nullptr;

    sgl::sgui::AbstractButton * mButtonClose = nullptr;
};

} // namespace game
