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

void ShortcutButton::HandleStateEnabled()
{
    if(IsChecked())
        SetState(CHECKED);
    else
        SetState(NORMAL);
}
void ShortcutButton::HandleStateDisabled()
{
     SetState(DISABLED);
}

void ShortcutButton::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonDown(event);

    OnButtonDown();
}

void ShortcutButton::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    OnButtonUp();
}

void ShortcutButton::HandleMouseOver()
{
    if(IsChecked())
        return ;

    SetState(MOUSE_OVER);
}

void ShortcutButton::HandleMouseOut()
{
    if(IsChecked())
        return ;

    SetState(NORMAL);
}

void ShortcutButton::HandleKeyDown(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        event.SetConsumed();

        OnButtonDown();
    }
}

void ShortcutButton::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        event.SetConsumed();

        OnButtonUp();

        Click();
    }
}

void ShortcutButton::HandleCheckedChanged(bool checked)
{
    SetState(checked ? CHECKED : NORMAL);
}

void ShortcutButton::SetState(VisualState state)
{
    if(mState == state)
        return ;

    mState = state;

    SetElements(state);
}

void ShortcutButton::OnButtonDown()
{
    if(IsCheckable())
        SetState(CHECKED);
    else
        SetState(PUSHED);
}

void ShortcutButton::OnButtonUp()
{
    if(IsCheckable())
        SetState(IsChecked() ? CHECKED : NORMAL);
    else
        SetState(NORMAL);
}

} // namespace game

