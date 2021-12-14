#include "Screens/ScreenSettings.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/ButtonDialogBack.h"
#include "Widgets/ButtonDialogContinue.h"
#include "Widgets/ButtonDialogSelect.h"
#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>
#include <sgui/Label.h>
#include <sgui/Stage.h>

namespace game
{

ScreenSettings::ScreenSettings(Game * game)
    : Screen(game)
{
    using namespace lib;

    InitSprites();

    const int screenW = graphic::Renderer::Instance()->GetWidth();
    const int screenH = graphic::Renderer::Instance()->GetHeight();

    mBg = new graphic::Image("data/img/space_bg.jpg");

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();
    graphic::Texture * tex = nullptr;
    graphic::Font * fnt = nullptr;

    // BUTTON BACK
    const int marginButton = 10;
    auto btnBack = new ButtonDialogBack;
    const int posBackX = screenW - btnBack->GetWidth() - marginButton;
    const int posBackY = screenH - btnBack->GetHeight() - marginButton;
    btnBack->SetPosition(posBackX, posBackY);

    btnBack->AddOnClickFunction([game]
    {
        game->RequestNextActiveState(StateId::MAIN_MENU);
    });
}

ScreenSettings::~ScreenSettings()
{
    lib::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenSettings::Update(float update)
{
}

void ScreenSettings::Render()
{
    mBg->Render();
}

void ScreenSettings::InitSprites()
{
    // NOTE this code will need to be moved to a dedicated screen to init data on start
    auto tm = lib::graphic::TextureManager::Instance();

    // CELLS
    std::vector<lib::core::Rectd> rects
    {
        // PANELS
        { 0, 0, 1102, 970 },
        { 1103, 0, 300, 260 },
        { 1103, 261, 300, 260 },
        { 1103, 522, 300, 260 },
        { 1103, 783, 300, 110 },
        { 1404, 0, 300, 370 },

        // ATTRIBUTES BAR
        { 1103, 894, 10, 12 },
        { 1114, 894, 10, 14 },
        { 1125, 894, 10, 16 },
        { 1136, 894, 10, 18 },
        { 1147, 894, 10, 20 },
        { 1158, 894, 10, 22 },
        { 1169, 894, 10, 24 },
        { 1180, 894, 10, 26 },
        { 1191, 894, 10, 28 },
        { 1202, 894, 10, 30 },

        // BACK BUTTON
        { 0, 971, 320, 50 },
        { 321, 971, 320, 50 },
        { 642, 971, 320, 50 },

        // SELECT BUTTON
        { 1404, 615, 250, 50 },
        { 1404, 666, 250, 50 },
        { 1404, 717, 250, 50 },
        { 1404, 768, 250, 50 },

        // CONTINUE BUTTON
        { 1404, 371, 300, 60 },
        { 1404, 432, 300, 60 },
        { 1404, 493, 300, 60 },
        { 1404, 554, 300, 60 }
    };

    tm->RegisterSprite(SpriteFileFactionSelection, rects);
}

} // namespace game
