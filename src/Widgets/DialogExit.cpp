#include "Widgets/DialogExit.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/GameButton.h"
#include "Widgets/GameUIData.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ImageButton.h>

namespace game
{

// ===== BUTTON 1 =====
class ButtonDialogExit : public GameButton
{
public:
    ButtonDialogExit(sgl::sgui::Widget * parent)
        : GameButton(SpriteFileDialogExit,
        { IND_DIA_EX_BTN_NORMAL, IND_DIA_EX_BTN_DISABLED,
          IND_DIA_EX_BTN_OVER, IND_DIA_EX_BTN_PUSHED, IND_DIA_EX_BTN_PUSHED },
        { 0xcadce5ff, 0x5a6266ff, 0xd7eaf4ff, 0xd7eaf4ff, 0xd7eaf4ff },
        parent)
    {
        using namespace sgl;

        const char * fileFont = "data/fonts/Lato-Regular.ttf";
        const int size = 24;

        auto fm = graphic::FontManager::Instance();
        graphic::Font * fnt = fm->GetFont(fileFont, size, graphic::Font::NORMAL);
        SetLabelFont(fnt);
    }
};

// ===== BUTTON X =====
class ButtonCloseDialogExit : public sgl::sgui::ImageButton
{
public:
    ButtonCloseDialogExit(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({ IND_DIA_EX_BTN_X_NORMAL, IND_DIA_EX_BTN_X_DISABLED,
                                 IND_DIA_EX_BTN_X_OVER, IND_DIA_EX_BTN_X_PUSHED,
                                 IND_DIA_EX_BTN_X_PUSHED }, SpriteFileDialogExit, parent)
    {
        SetShortcutKey(sgl::core::KeyboardEvent::KEY_ESC);
    }
};

// ===== DIALOG =====
DialogExit::DialogExit(Game * game)
{
    using namespace sgl;

    auto fm = sgl::graphic::FontManager::Instance();
    auto tm = sgl::graphic::TextureManager::Instance();

    const int marginL = 40;
    const int marginT = 8;

    // BACKGROUND
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileDialogExit, IND_NE_DIALOG_BG);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();
    SetSize(w, h);

    // BUTTON CLOSE
    mButtonClose = new ButtonCloseDialogExit(this);
    mButtonClose->SetX(GetWidth() - mButtonClose->GetWidth());

    // BUTTON PLANET MAP
    const int btnY0 = 78;
    const int marginBtnV = 38;
    int btnX = 0;
    int btnY = btnY0;

    auto btn = new ButtonDialogExit(this);
    btn->SetLabel("PLANET MAP");

    btnX = (w - btn->GetWidth()) / 2;
    btn->SetPosition(btnX, btnY);

    btn->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::PLANET_MAP);
    });

    btnY += btn->GetHeight() + marginBtnV;

    // BUTTON MAIN MENU
    btn = new ButtonDialogExit(this);
    btn->SetLabel("MAIN MENU");
    btn->SetPosition(btnX, btnY);

    btn->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });

    btnY += btn->GetHeight() + marginBtnV;

    // BUTTON EXIT
    btn = new ButtonDialogExit(this);
    btn->SetLabel("EXIT");
    btn->SetPosition(btnX, btnY);

    btn->AddOnClickFunction([game]
    {
        game->Exit();
    });
}

void DialogExit::SetFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

void DialogExit::HandlePositionChanged()
{
    SetPositions();
}

void DialogExit::SetPositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBg->SetPosition(x0, y0);
}
} // namespace game
