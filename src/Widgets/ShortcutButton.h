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
    void HandleKeyDown(lib::core::KeyboardEvent & event) override;
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:
    int mShortcutKey = -1;
};

} // namespace game
