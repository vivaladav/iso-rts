#include "Widgets/PanelGameOver.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <sgui/Label.h>

namespace game
{

PanelGameOver::PanelGameOver(Game * game, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    FontManager * fm = FontManager::Instance();

    Font * fontName = fm->GetFont("data/fonts/OpenSans.ttf", 128, Font::BOLD);

    Label * labelName = new Label("GAME OVER", fontName, this);
    labelName->SetColor(0x060606FF);

    const int marginH = 50;

    // -- BUTTON MAIN MENU --
    ButtonMainMenu * button = new ButtonMainMenu("MAIN MENU", this);

    button->SetOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    const int buttonX = (labelName->GetWidth() - button->GetWidth()) * 0.5f;
    const int buttonY = labelName->GetHeight() + marginH;
    button->SetPosition(buttonX, buttonY);
}

} // namespace game
