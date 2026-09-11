#include "Widgets/DialogNewGame.h"

#include "Game.h"
#include "GameConstants.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonDialogBack.h"
#include "Widgets/ButtonDialogOk.h"
#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/core/event/KeyboardEvent.h>
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
#include <sgl/sgui/AbstractButtonsGroup.h>
#include <sgl/sgui/ComboBox.h>
#include <sgl/sgui/ComboBoxItem.h>
#include <sgl/sgui/Image.h>
#include <sgl/sgui/ImageButton.h>
#include <sgl/sgui/Label.h>
#include <sgl/sgui/TextArea.h>
#include <sgl/utilities/StringManager.h>

namespace game
{

DialogNewGame::DialogNewGame::DialogNewGame(Game * game)
    : mGame(game)
{
    using namespace sgl;

    auto fm = graphic::FontManager::Instance();
    auto tm = graphic::TextureManager::Instance();
    auto sm = utilities::StringManager::Instance();

    // BACKGROUND
    const int w = 1882;
    graphic::Texture * tex;

    tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_BG_L);
    mBgL = new graphic::Image(tex);
    RegisterRenderable(mBgL);

    const int wL = mBgL->GetWidth();
    const int h = mBgL->GetHeight();

    tex = tm->GetSprite(SpriteFileDialogNewGame, ID_DLG_NEWG_BG_R);
    mBgR = new graphic::Image(tex);
    RegisterRenderable(mBgR);

    const int wR = mBgR->GetWidth();

    tex = tm->GetTexture(SpriteFileDialogNewGameExp);
    tex->SetScaleMode(graphic::TSCALE_NEAREST);
    mBgC = new graphic::Image(tex);
    RegisterRenderable(mBgC);

    const int wC = w - wL - wR;
    mBgC->SetWidth(wC);

    SetSize(w, h);

    // BUTTON CLOSE
    mButtonClose = new ButtonDialogBack(this);
    mButtonClose->SetX(w - mButtonClose->GetWidth());

    mButtonClose->AddOnClickFunction([this]
    {
        mGame->RequestNextActiveState(StateId::LEAVE_GAME);
    });

    // TITLE
    auto fontTitle = fm->GetFont(WidgetsConstants::FontFileDialogTitle,
                                 WidgetsConstants::FontSizeDialogTitle, graphic::Font::NORMAL);

    auto title = new sgui::Label(sm->GetCString("NEW_GAME"), fontTitle, this);

    const int titleX = WidgetsConstants::MarginDialogTitleL;
    const int titleY = (WidgetsConstants::DialogTitleBarH - title->GetHeight()) / 2;
    title->SetPosition(titleX, titleY);
    title->SetColor(WidgetsConstants::colorDialogTitle);

    // BUTTON CONTINUE
    const int marginB = 20;

    mButtonContinue = new ButtonDialogOk(sm->GetCString("CONTINUE"), this);
    mButtonContinue->SetEnabled(false);
    mButtonContinue->SetPosition(w - mButtonContinue->GetWidth() - WidgetsConstants::MarginDialogContentR,
                            h - mButtonContinue->GetHeight() - marginB);

    mButtonContinue->AddOnClickFunction([this]
    {
        mGame->RequestNextActiveState(StateId::FACTION_SEL);
    });
}

DialogNewGame::~DialogNewGame()
{
    delete mButtonsFact;
}

void DialogNewGame::HandlePositionChanged()
{
    SetPositions();
}

void DialogNewGame::SetPositions()
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