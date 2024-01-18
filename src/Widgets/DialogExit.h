#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }
    namespace sgui { class AbstractButton; }
}

namespace game
{

class Game;
class Screen;

class DialogExit : public sgl::sgui::Widget
{
public:
    DialogExit(Game * game, Screen * screen);

    void SetFunctionOnShowingDialogSettings(const std::function<void()> & f);
    void SetFunctionOnHidingDialogSettings(const std::function<void()> & f);

    void SetFunctionOnClose(const std::function<void()> & f);

protected:
    void HandlePositionChanged() override;

private:
    void SetPositions();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Text * mTitle = nullptr;
    sgl::sgui::AbstractButton * mButtonClose = nullptr;

    std::function<void()> mOnShowingSettings;
    std::function<void()> mOnHidingSettings;
};

} // namespace game
