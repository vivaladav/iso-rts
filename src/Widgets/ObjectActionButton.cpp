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
    : ShortcutButton(shortcutKey, parent)
    , mBody(new lib::graphic::Image)
    , mIcon(new lib::graphic::Image)
{
    using namespace lib::graphic;

    assert(icon < NUM_ACTION_ICONS);

    RegisterRenderable(mBody);
    RegisterRenderable(mIcon);

    // -- SET ICON TEXTURE --
    auto tm = TextureManager::Instance();
    Texture * tex = nullptr;

    const SpriteIdObjActionButton texId[] =
    {
        IND_BUTTON_ICON_UNITS,
        IND_BUTTON_ICON_MOVE,
        IND_BUTTON_ICON_ATTACK,
        IND_BUTTON_ICON_CONQUER_CELL,
        IND_BUTTON_ICON_BUILD_WALL,
        IND_BUTTON_ICON_BUILD_STRUCT,
        IND_BUTTON_ICON_UPGRADE,
        IND_BUTTON_ICON_CANCEL
    };

    tex = tm->GetSprite(SpriteFileObjActionButton, texId[icon]);
    mIcon->SetTexture(tex);

    // -- CREATE SHORTCUT TEXT --
    auto fm = FontManager::Instance();
    auto font = fm->GetFont("data/fonts/Lato-Regular.ttf", 12, Font::BOLD);
    mShortcut = new Text(shortcut, font, true);

    RegisterRenderable(mShortcut);

    // set initial visual state
    SetState(NORMAL);
}

ObjectActionButton::~ObjectActionButton()
{
    delete mBody;

    delete mIcon;

    delete mShortcut;
}

void ObjectActionButton::SetElements(VisualState state)
{
    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    const unsigned char alphaEn = 255;

    mIcon->SetAlpha(alphaEn);
    mShortcut->SetAlpha(alphaEn);

    switch(state)
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
        {
            tex = tm->GetSprite(SpriteFileObjActionButton, IND_BUTTON_DISABLED);
            mBody->SetTexture(tex);

            const unsigned char alphaDis = 128;
            mIcon->SetAlpha(alphaDis);
            mShortcut->SetAlpha(alphaDis);
        }
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
    PushButton::OnRender();

    mIcon->Render();

    mShortcut->Render();
}

} // namespace game

