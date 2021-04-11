#include "Widgets/ObjectActionButton.h"

#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/TextureManager.h>

#include <cassert>

namespace game
{

ObjectActionButton::ObjectActionButton(ActionIcon icon, const char * shortcut, lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
    , mBody(new lib::graphic::Image)
    , mIcon(new lib::graphic::Image)
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

        case CONQUER:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_CONQUER);
            mIcon->SetTexture(tex);
        break;

        case BUILD_WALL:
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_ICON_BUILD_WALL);
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

    if(!IsEnabled())
        return ;

    if(IsCheckable())
        SetElements(CHECKED);
    else
        SetElements(PUSHED);
}

void ObjectActionButton::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    if(!IsEnabled())
        return ;

    if(IsCheckable())
        SetElements(IsChecked() ? CHECKED : NORMAL);
    else
        SetElements(NORMAL);
}

void ObjectActionButton::HandleMouseOver()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetElements(MOUSE_OVER);
}

void ObjectActionButton::HandleMouseOut()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetElements(NORMAL);
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
   const int iconBoxW = 50;
   const int iconBoxH = 50;

   const int iconX = x + (iconBoxW - mIcon->GetWidth()) * 0.5f;
   const int iconY = y + (iconBoxH - mIcon->GetHeight()) * 0.5f;

   mIcon->SetPosition(iconX, iconY);

   // SHORTCUT
   const int shortcutX0 = 42;
   const int shortcutY0 = 42;

   const int shortcutBoxW = 14;
   const int shortcutBoxH = 14;

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

} // namespace game

