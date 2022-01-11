#pragma once

#include "Widgets/ShortcutButton.h"

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

class ObjectActionButton : public ShortcutButton
{
public:
    enum ActionIcon : unsigned int
    {
        UNITS,
        MOVE,
        ATTACK,
        CONQUER_CELL,
        BUILD_WALL,
        BUILD_STRUCT,
        UPGRADE,
        CANCEL,

        NUM_ACTION_ICONS
    };

public:
    ObjectActionButton(ActionIcon icon, const char * shortcut,
                       int shortcutKey, lib::sgui::Widget * parent);
    ~ObjectActionButton();

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

    void HandlePositionChanged() override;

private:
    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Image * mIcon = nullptr;
    lib::graphic::Text * mShortcut = nullptr;
};

} // namespace game
