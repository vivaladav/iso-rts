#include "Widgets/ShortcutButton.h"

#include "Widgets/GameUIData.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>

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

