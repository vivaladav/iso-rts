#include "Widgets/DialogCredits.h"

#include "Game.h"
#include "GameConstants.h"
#include "Tutorial/TutorialManager.h"
#include "Widgets/ButtonDialogClose.h"
#include "Widgets/ButtonPanelTab.h"
#include "Widgets/GameSliderH.h"
#include "Widgets/GameUIData.h"
#include "Widgets/SettingsCheckbox.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/GraphicConstants.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Window.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>
#include <sgl/sgui/ButtonsGroup.h>
#include <sgl/sgui/ComboBox.h>
#include <sgl/sgui/ComboBoxItem.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/ImageButton.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/Stage.h>
#include <sgl/utilities/StringManager.h>

#include <cassert>
#include <iostream>
#include <sstream>

// anonymous namespace for local "private" classes
namespace
{

} // namespace

namespace game
{

// ====== DIALOG CREDITS ======
DialogCredits::DialogCredits(Game * game)
    : mGame(game)
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto sm = utilities::StringManager::Instance();
    auto tm = graphic::TextureManager::Instance();

    // BACKGROUND
    const int w = 1202;
    graphic::Texture * tex;

    tex = tm->GetSprite(SpriteFileDialogCredits, ID_DLG_CRE_BG_L);
    mBgL = new graphic::Image(tex);
    RegisterRenderable(mBgL);

    const int wL = mBgL->GetWidth();
    const int h = mBgL->GetHeight();

    tex = tm->GetSprite(SpriteFileDialogCredits, ID_DLG_CRE_BG_R);
    mBgR = new graphic::Image(tex);
    RegisterRenderable(mBgR);

    const int wR = mBgR->GetWidth();

    tex = tm->GetTexture(SpriteFileDialogCreditsExp);
    tex->SetScaleMode(graphic::TSCALE_NEAREST);
    mBgC = new graphic::Image(tex);
    RegisterRenderable(mBgC);

    const int wC = w - wL - wR;
    mBgC->SetWidth(wC);

    SetSize(w, h);

    // BUTTON CLOSE
    mButtonClose = new ButtonDialogClose(this);
    mButtonClose->SetX(w - mButtonClose->GetWidth());

    mButtonClose->AddOnClickFunction([this]
        {
            mGame->SaveSettings();
        });

    // TITLE
    auto fontTitle = fm->GetFont(WidgetsConstants::FontFileDialogTitle,
                                 WidgetsConstants::FontSizeDialogTitle, graphic::Font::NORMAL);

    mTitle = new sgui::Label(sm->GetCString("CREDITS"), fontTitle, this);

    const int titleX = WidgetsConstants::MarginDialogTitleL;
    const int titleY = (WidgetsConstants::DialogTitleBarH - mTitle->GetHeight()) / 2;
    mTitle->SetPosition(titleX, titleY);
    mTitle->SetColor(WidgetsConstants::colorDialogTitle);
}

DialogCredits::~DialogCredits()
{
}

void DialogCredits::AddOnCloseClickedFunction(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

void DialogCredits::HandlePositionChanged()
{
    SetPositions();
}

int DialogCredits::GetMouseButtonIndex(int buttonId) const
{
    return buttonId - 1;
}

void DialogCredits::SetPositions()
{
    const int y = GetScreenY();
    int x = GetScreenX();

    // BACKGROUND
    mBgL->SetPosition(x, y);
    x += mBgL->GetWidth();

    mBgC->SetPosition(x, y);
    x += mBgC->GetWidth();

    mBgR->SetPosition(x, y);
}

} // namespace game
