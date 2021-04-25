#include "Widgets/ObjectActionButton.h"

#include "Widgets/GameUIData.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/TextureManager.h>

#include <cassert>

namespace game
{

ObjectActionButton::ObjectActionButton(ActionIcon icon, const char * shortcut,
                                       int shortcutKey, lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
    , mBody(new lib::graphic::Image)
    , mIcon(new lib::graphic::Image)
    , mShortcutKey(shortcutKey)
{
    using namespace lib::graphic;

    assert(icon < NUM_ACTION_ICONS);

    SetCurrBg(mBody);

    SetElements(NORMAL);

    // -- SET ICON TEXTURE --
    auto tm = TextureManager::Instance();
    Texture * tex = nullptr;

    switch(icon)
    {
        case UNITS:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_UNITS);
            mIcon->SetTexture(tex);
        break;

        case MOVE:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_MOVE);
            mIcon->SetTexture(tex);
        break;

        case ATTACK:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_ATTACK);
            mIcon->SetTexture(tex);
        break;

        case CONQUER_CELL:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_CONQUER_CELL);
            mIcon->SetTexture(tex);
        break;

        case BUILD_WALL:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_BUILD_WALL);
            mIcon->SetTexture(tex);
        break;

        case UPGRADE:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_UPGRADE);
            mIcon->SetTexture(tex);
        break;

        case CANCEL:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_CANCEL);
            mIcon->SetTexture(tex);
        break;

        // this should not happen
        default:

        break;
    }

    // -- CREATE SHORTCUT TEXT --
    auto fm = FontManager::Instance();
    auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 12, Font::BOLD);
    mShortcut = new Text(shortcut, font, true);
}

ObjectActionButton::~ObjectActionButton()
{
    delete mBody;

    delete mIcon;

    delete mShortcut;
}

void ObjectActionButton::HandleStateEnabled()
{
    if(IsChecked())
        SetElements(CHECKED);
    else
        SetElements(NORMAL);
}
void ObjectActionButton::HandleStateDisabled()
{
     SetElements(DISABLED);
}

void ObjectActionButton::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonDown(event);

    OnButtonDown();
}

void ObjectActionButton::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    OnButtonUp();
}

void ObjectActionButton::HandleMouseOver()
{
    if(IsChecked())
        return ;

    SetElements(MOUSE_OVER);
}

void ObjectActionButton::HandleMouseOut()
{
    if(IsChecked())
        return ;

    SetElements(NORMAL);
}

void ObjectActionButton::HandleKeyDown(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        event.SetConsumed();

        OnButtonDown();
    }
}

void ObjectActionButton::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    if(event.GetKey() == mShortcutKey)
    {
        event.SetConsumed();

        OnButtonUp();

        Click();
    }
}

void ObjectActionButton::HandleCheckedChanged(bool checked)
{
    SetElements(checked ? CHECKED : NORMAL);
}

void ObjectActionButton::SetElements(int index)
{
    if(mState == index)
        return ;

    mState = index;

    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    switch (mState)
    {
        case NORMAL:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_NORMAL);
            mBody->SetTexture(tex);
        break;

        case MOUSE_OVER:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_MOUSE_OVER);
            mBody->SetTexture(tex);
        break;

        case PUSHED:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_PUSHED);
            mBody->SetTexture(tex);
        break;

        case CHECKED:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_CHECKED);
            mBody->SetTexture(tex);
        break;

        // disabled by default
        default:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_DISABLED);
            mBody->SetTexture(tex);
        break;
    }

    // reset BG to make changes visible
    SetCurrBg(mBody);
}

void ObjectActionButton::HandlePositionChanged()
{
   PushButton::HandlePositionChanged();

   const int x = GetScreenX();
   const int y = GetScreenY();

   // ICON
   const int iconBoxW = 56;
   const int iconBoxH = 56;

   const int iconX = x + (iconBoxW - mIcon->GetWidth()) * 0.5f;
   const int iconY = y + (iconBoxH - mIcon->GetHeight()) * 0.5f;

   mIcon->SetPosition(iconX, iconY);

   // SHORTCUT
   const int shortcutX0 = 48;
   const int shortcutY0 = 48;

   const int shortcutBoxW = 16;
   const int shortcutBoxH = 16;

   const int shortcutX = x + shortcutX0 + (shortcutBoxW - mShortcut->GetWidth()) * 0.5f;
   const int shortcutY = y + shortcutY0 + (shortcutBoxH - mShortcut->GetHeight()) * 0.5f;

   mShortcut->SetPosition(shortcutX, shortcutY);
}

void ObjectActionButton::OnRender()
{
    mBody->Render();

    mIcon->Render();

    mShortcut->Render();
}

void ObjectActionButton::OnButtonDown()
{
    if(IsCheckable())
        SetElements(CHECKED);
    else
        SetElements(PUSHED);
}

void ObjectActionButton::OnButtonUp()
{
    if(IsCheckable())
        SetElements(IsChecked() ? CHECKED : NORMAL);
    else
        SetElements(NORMAL);
}

} // namespace game

