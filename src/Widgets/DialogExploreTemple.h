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

    namespace sgui { class Label; }
}

namespace game
{

class Game;
class GameButton;
class Screen;

class DialogExploreTemple : public sgl::sgui::Widget
{
public:
    DialogExploreTemple();

    void SetFunctionOnClose(const std::function<void()> & f);

protected:
    void HandlePositionChanged() override;

private:
    void SetPositions();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Image * mLineH1 = nullptr;
    sgl::graphic::Image * mLineH2 = nullptr;

    sgl::graphic::Image * mIconMoney = nullptr;
    sgl::graphic::Image * mIconMaterial = nullptr;
    sgl::graphic::Image * mIconBlobs = nullptr;
    sgl::graphic::Image * mIconDiamonds = nullptr;

    sgl::graphic::Text * mHeaderInvest = nullptr;
    sgl::graphic::Text * mHeaderTime = nullptr;
    sgl::graphic::Text * mHeaderSuccess = nullptr;
    sgl::sgui::Label * mLabelTime = nullptr;
    sgl::sgui::Label * mLabelSuccess = nullptr;

    GameButton * mBtnAbort = nullptr;
    GameButton * mBtnExplore = nullptr;
};

} // namespace game
