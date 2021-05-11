#include "Widgets/ButtonBuildNewUnit.h"

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

ButtonBuildNewUnit::ButtonBuildNewUnit(const char * shortcut,
                                       int shortcutKey, lib::sgui::Widget * parent)
    : ShortcutButton(shortcutKey, parent)
    , mBody(new lib::graphic::Image)
{
    using namespace lib::graphic;

    // background
    RegisterRenderable(mBody);

    SetCurrBg(mBody);

    // label
    auto fm = FontManager::Instance();
    auto font = fm->GetFont("data/fonts/Lato-Bold.ttf", 16, Font::NORMAL);
    mLabel  = new Text("BUILD", font, true);
    RegisterRenderable(mLabel);

    // -- CREATE SHORTCUT TEXT --
    font = fm->GetFont("data/fonts/Lato-Bold.ttf", 12, Font::NORMAL);
    mShortcut = new Text(shortcut, font, true);
    RegisterRenderable(mShortcut);

    // set initial visual state
    SetState(NORMAL);
}

ButtonBuildNewUnit::~ButtonBuildNewUnit()
{
    delete mBody;

    delete mLabel;

    delete mShortcut;
}

void ButtonBuildNewUnit::SetElements(VisualState state)
{
    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    mShortcut->SetColor(0xd5daddff);

    switch(state)
    {
        case NORMAL:
            tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BUTTON_NORMAL);
            mBody->SetTexture(tex);

            mLabel->SetColor(0xe3e6e8ff);
        break;

        case MOUSE_OVER:
            tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BUTTON_MOUSE_OVER);
            mBody->SetTexture(tex);

            mLabel->SetColor(0xf1f2f4ff);
        break;

        case PUSHED:
            tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BUTTON_PUSHED);
            mBody->SetTexture(tex);

            mLabel->SetColor(0xabb4baff);
        break;

        case CHECKED:
            tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BUTTON_CHECKED);
            mBody->SetTexture(tex);

            mLabel->SetColor(0xc2c2a3ff);
        break;

        // disabled by default
        default:
        {
            tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BUTTON_DISABLED);
            mBody->SetTexture(tex);

            const unsigned int colorDis = 0x454f54ff;

            mLabel->SetColor(colorDis);
            mShortcut->SetColor(colorDis);
        }
        break;
    }

    // reset BG to make changes visible
    SetCurrBg(mBody);
}

void ButtonBuildNewUnit::HandlePositionChanged()
{
   PushButton::HandlePositionChanged();

   const int x = GetScreenX();
   const int y = GetScreenY();

   // LABEL
   const int shortcutBoxOverflow = 6;
   const int labelX = x + (GetWidth() - shortcutBoxOverflow - mLabel->GetWidth()) * 0.5f;
   const int labelY = y + (GetHeight() - shortcutBoxOverflow - mLabel->GetHeight()) * 0.5f;

   mLabel->SetPosition(labelX, labelY);

   // SHORTCUT
   const int shortcutBoxW = 14;
   const int shortcutBoxH = 14;

   const int shortcutX = x + GetWidth() - shortcutBoxW + (shortcutBoxW - mShortcut->GetWidth()) * 0.5f;
   const int shortcutY = y + GetHeight() - shortcutBoxH + (shortcutBoxH - mShortcut->GetHeight()) * 0.5f;

   mShortcut->SetPosition(shortcutX, shortcutY);
}

void ButtonBuildNewUnit::OnRender()
{
    mBody->Render();

    mLabel->Render();

    mShortcut->Render();
}

} // namespace game

