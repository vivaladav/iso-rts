#pragma once

#include <sgui/PushButton.h>

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

class ShortcutButton : public lib::sgui::PushButton
{
public:
    ShortcutButton(int shortcutKey, lib::sgui::Widget * parent);

protected:
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

    void SetState(VisualState state);

    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void HandleMouseButtonDown(lib::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void HandleMouseOver() override;
    void HandleMouseOut() override;

    void HandleKeyDown(lib::core::KeyboardEvent & event) override;
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

    void HandleCheckedChanged(bool checked) override;

    virtual void SetElements(VisualState state) = 0;

private:
    void OnButtonDown();
    void OnButtonUp();

private:
    VisualState mState = NULL_STATE;

    int mShortcutKey = -1;
};

} // namespace game
