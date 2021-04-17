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

class ObjectActionButton : public lib::sgui::PushButton
{
public:
    enum ActionIcon : unsigned int
    {
        UNITS,
        MOVE,
        ATTACK,
        CONQUER,
        BUILD_WALL,
        UPGRADE,

        NUM_ACTION_ICONS
    };

public:
    ObjectActionButton(ActionIcon icon, const char * shortcut,
                       int shortcutKey, lib::sgui::Widget * parent);
    ~ObjectActionButton();

protected:
    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void HandleMouseButtonDown(lib::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void HandleMouseOver() override;
    void HandleMouseOut() override;

    void HandleKeyDown(lib::core::KeyboardEvent & event) override;
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

    void HandleCheckedChanged(bool checked) override;

    void SetElements(int index);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

    void OnButtonDown();
    void OnButtonUp();

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

    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Image * mIcon = nullptr;
    lib::graphic::Text * mShortcut = nullptr;

    int mState = NULL_STATE;

    int mShortcutKey = -1;
};

} // namespace game
