#include "Widgets/PanelGameOver.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonMainMenu.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/sgui/Label.h>

namespace game
{

PanelGameOver::PanelGameOver(Game * game, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
{
    using namespace sgl::graphic;
    using namespace sgl::sgui;

    FontManager * fm = FontManager::Instance();

    Font * fontName = fm->GetFont("Lato-Bold.ttf", 128, Font::NORMAL);

    Label * labelName = new Label("GAME OVER", fontName, this);
    labelName->SetColor(0x060606FF);

    const int marginH = 50;

    // -- BUTTON MAIN MENU --
    ButtonMainMenu * button = new ButtonMainMenu("MAIN MENU", this);

    button->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    const int buttonX = (labelName->GetWidth() - button->GetWidth()) * 0.5f;
    const int buttonY = labelName->GetHeight() + marginH;
    button->SetPosition(buttonX, buttonY);
}

} // namespace game
