#pragma once

#include <sgl/sgui/PushButton.h>

#include <array>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class ButtonMainMenuWishlist : public sgl::sgui::PushButton
{

public:
    ButtonMainMenuWishlist(sgl::sgui::Widget * parent);

private:
    void HandlePositionChanged() override;

    void OnStateChanged(sgl::sgui::PushButton::VisualState state) override;

    void PositionElements();

private:
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Image * mIcon = nullptr;
    sgl::graphic::Text * mText = nullptr;
};

} // namespace game
