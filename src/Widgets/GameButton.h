#pragma once

#include <sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic
    {
        class Image;
    }
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

        NUM_VISUAL_STATES
    };

public:
    GameButton(const char * text,
               const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               lib::sgui::Widget * parent);

    ~GameButton();

protected:
    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void HandleMouseButtonDown(lib::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void HandleMouseOver() override;
    void HandleMouseOut() override;

private:
    void SetElements(int index);

private:
    std::array<lib::graphic::Image *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;
};

} // namespace game
