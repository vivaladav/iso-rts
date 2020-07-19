#pragma once

#include <sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class GameButton : public lib::sgui::PushButton
{
public:
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

public:
    GameButton(lib::sgui::Widget * parent);

    GameButton(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               lib::sgui::Widget * parent);

    ~GameButton();

    void SetData(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
                 int state);

protected:
    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void HandleMouseButtonDown(lib::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void HandleMouseOver() override;
    void HandleMouseOut() override;

    void HandleCheckedChanged(bool checked) override;

    void SetElements(int index);

private:
    std::array<lib::graphic::Image *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    int mState = NULL_STATE;
};

} // namespace game
