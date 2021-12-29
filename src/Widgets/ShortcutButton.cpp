#include "Widgets/ShortcutButton.h"

#include "Widgets/GameUIData.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/TextureManager.h>

#include <cassert>

namespace game
{

ShortcutButton::ShortcutButton(int shortcutKey,
                               lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
    , mShortcutKey(shortcutKey)
{
}

void ShortcutButton::HandleKeyDown(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        HandleButtonDown();

        event.SetConsumed();
    }
}

void ShortcutButton::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        HandleButtonUp();

        event.SetConsumed();
    }
}

} // namespace game

