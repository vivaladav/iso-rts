#include "Widgets/ObjectActionButton.h"

#include "Widgets/GameUIData.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/TextureManager.h>

#include <cassert>

namespace game
{

ObjectActionButton::ObjectActionButton(ActionIcon icon, const char * shortcut,
                                       int shortcutKey, sgl::sgui::Widget * parent)
    : ShortcutButton(shortcutKey, parent)
    , mBody(new sgl::graphic::Image)
    , mIcon(new sgl::graphic::Image)
{
    using namespace sgl::graphic;

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
        IND_BUTTON_ICON_OPEN_GATE,
        IND_BUTTON_ICON_CLOSE_GATE,
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

void ObjectActionButton::OnStateChanged(VisualState state)
{
    const unsigned int texIds[NUM_VISUAL_STATES] =
    {
        IND_BUTTON_NORMAL,
        IND_BUTTON_DISABLED,
        IND_BUTTON_MOUSE_OVER,
        IND_BUTTON_PUSHED,
        IND_BUTTON_CHECKED,
    };

    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileObjActionButton, texIds[state]);
    mBody->SetTexture(tex);

    SetSize(mBody->GetWidth(), mBody->GetHeight());

    // update shortcut label alpha
    const unsigned char alphaEn = 255;
    const unsigned char alphaDis = 128;
    const unsigned char alphaCont = DISABLED == state ? alphaDis : alphaEn;
    mShortcut->SetAlpha(alphaCont);

    // icon
    mIcon->SetAlpha(alphaCont);
}

void ObjectActionButton::HandlePositionChanged()
{
   ShortcutButton::HandlePositionChanged();

   const int x = GetScreenX();
   const int y = GetScreenY();

   // position BG
   mBody->SetPosition(x, y);

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

} // namespace game

