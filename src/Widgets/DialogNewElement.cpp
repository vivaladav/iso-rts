#include "Widgets/DialogNewElement.h"

#include "Player.h"
#include "GameObjects/Unit.h"
#include "GameObjects/UnitData.h"
#include "Widgets/ButtonCloseDialog.h"
#include "Widgets/GameUIData.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>
#include <sgui/Label.h>

namespace game
{

DialogNewElement::DialogNewElement(Player * player)
//    : mPlayer(player)
{
    using namespace lib::sgui;

    auto tm = lib::graphic::TextureManager::Instance();

    // BACKGROUND
    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileNewElementDialog, IND_NE_DIALOG_BG);
    auto imgBg = new Image(tex, this);

    // CLOSE BUTTON
    const int buttonSide = 20;

//    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_ICON_CLOSE);
//    mButtonClose = new ButtonCloseDialog(tex, buttonSide, buttonSide, this);
}

DialogNewElement::~DialogNewElement()
{

}

void DialogNewElement::SetFunctionOnBuild(const std::function<void()> & f)
{

}

void DialogNewElement::SetFunctionOnClose(const std::function<void()> & f)
{

}

void DialogNewElement::HandleKeyUp(lib::core::KeyboardEvent & event)
{
    // ESC -> close dialog
    if(event.GetKey() == lib::core::KeyboardEvent::KEY_ESC)
    {

    }
}

} // namespace game
