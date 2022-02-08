#pragma once

#include <sgl/sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class ButtonMainMenuWishlist : public lib::sgui::PushButton
{

public:
    ButtonMainMenuWishlist(lib::sgui::Widget * parent);

private:
    void HandlePositionChanged() override;

    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

    void PositionElements();

private:
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Image * mIcon = nullptr;
    lib::graphic::Text * mText = nullptr;
};

} // namespace game
