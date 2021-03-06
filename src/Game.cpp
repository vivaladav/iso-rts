#include "Game.h"

#include "GameConstants.h"
#include "Player.h"
#include "GameObjects/UnitsDataRegistry.h"
#include "States/StatesIds.h"
#include "States/StateFactionSelection.h"
#include "States/StateGame.h"
#include "States/StateMainMenu.h"
#include "States/StateNewGame.h"
#include "States/StateTest.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/GraphicConstants.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/TextureManager.h>
#include <graphic/Window.h>
#include <sgui/Stage.h>
#include <utilities/StateManager.h>

#include <iostream>

namespace game
{

Game::Game(int argc, char * argv[])
    : lib::core::Application(argc, argv)
    , mMapFiles
    {
        "data/maps/20x20-empty.map",
        "data/maps/40x40-01.map",
        "data/maps/60x60-01.map"
    }
    , mUnitsRegistry(new UnitsDataRegistry)
    , mLocalFaction(NO_FACTION)
{
    using namespace lib::graphic;

    mWin = Window::Create("Virtueror - v. 0.0.1", 1600, 900);
    mRenderer = Renderer::Create(mWin);
    mRenderer->SetLogicalSize(1920, 1080);

    TextureManager::Instance()->SetNewTextureQuality(TextureQuality::BEST);

    FontManager * fm = FontManager::Create();

    // -- State Manager --
    mStateMan = new lib::utilities::StateManager;

    mStateMan->AddState(new StateFactionSelection(this));
    mStateMan->AddState(new StateGame(this));
    mStateMan->AddState(new StateTest(this));
    mStateMan->AddState(new StateMainMenu(this));
    mStateMan->AddState(new StateNewGame(this));

    mStateMan->RequestNextActiveState(StateId::MAIN_MENU);

    // -- SGUI Stage --
    mStage = lib::sgui::Stage::Create();
    AddKeyboardListener(mStage);
    AddMouseListener(mStage);

    mFontGui = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);
    mStage->SetDefaultFont(mFontGui);
}

Game::~Game()
{
    delete mUnitsRegistry;

    ClearPlayers();

    delete mStateMan;

    lib::sgui::Stage::Destroy();

    lib::graphic::FontManager::Destroy();

    lib::graphic::Renderer::Destroy();
    lib::graphic::Window::Destroy();
}

void Game::RequestNextActiveState(StateId sid) { mStateMan->RequestNextActiveState(sid); }

void Game::Update(float delta)
{
    mRenderer->Clear(mClearR, mClearG, mClearB, mClearA);

    mStateMan->UpdateActive();

    auto * state = static_cast<BaseGameState *>(mStateMan->GetActiveState());

    state->Update(delta);

    state->Render();
    mStage->Render();

    mRenderer->Finalize();
}

Player * Game::AddPlayer(const char * name, int pid)
{
    if(mPlayers.size() == MAX_NUM_PLAYERS)
        return nullptr;

    Player * p = new Player(name, pid);

    mPlayers.push_back(p);

    return p;
}

void Game::ClearPlayers()
{
    for(Player * p : mPlayers)
        delete p;

    mPlayers.clear();
}

} // namespace game
