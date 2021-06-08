#pragma once

#include <sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class ButtonMainMenuSocial : public lib::sgui::PushButton
{

public:
    ButtonMainMenuSocial(unsigned int icon, lib::sgui::Widget * parent);
    ~ButtonMainMenuSocial();

protected:
    void HandlePositionChanged() override;

    void HandleMouseButtonDown(lib::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void HandleMouseOver() override;
    void HandleMouseOut() override;

    void OnRender() override;

    void SetElements(int index);

private:
    void PositionElements();

private:
    enum VisualState : int
    {
        NORMAL = 0,
        DISABLED,
        MOUSE_OVER,
        PUSHED,
        CHECKED,

        NUM_VISUAL_STATES,

        NULL_STATE
    };

private:
    std::array<lib::graphic::Image *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    lib::graphic::Image * mIcon = nullptr;

    int mState = NULL_STATE;
};

} // namespace game
