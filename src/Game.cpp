#include "Game.h"

#include "States/StatesIds.h"
#include "States/StateGame.h"
#include "States/StateMainMenu.h"
#include "States/StateTest.h"

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
{
    using namespace lib::graphic;

    mWin = Window::Create("Iso RTS - v. 0.0.1", 1280, 720);
    mRenderer = Renderer::Create(mWin);
    mRenderer->SetLogicalSize(1920, 1080);

    TextureManager::Instance()->SetNewTextureQuality(TextureQuality::BEST);

    // -- State Manager --
    mStateMan = new lib::utilities::StateManager;

    mStateMan->AddState(new StateGame(this));
    mStateMan->AddState(new StateTest(this));
    mStateMan->AddState(new StateMainMenu(this));

    mStateMan->RequestNextActiveState(StateId::MAIN_MENU);

    // -- SGUI Stage --
    mStage = lib::sgui::Stage::Create();
    AddKeyboardListener(mStage);
    AddMouseListener(mStage);
}

Game::~Game()
{
    RemoveKeyboardListener(mStage);
    RemoveMouseListener(mStage);
    lib::sgui::Stage::Destroy();

    delete mStateMan;

    lib::graphic::Renderer::Destroy();
    lib::graphic::Window::Destroy();
}

void Game::RequestNextActiveState(StateId sid) { mStateMan->RequestNextActiveState(sid); }

void Game::Update()
{
    mRenderer->Clear(mClearR, mClearG, mClearB, mClearA);

    mStateMan->UpdateActive();

    auto * state = static_cast<BaseGameState *>(mStateMan->GetActiveState());

    state->Update();

    state->Render();
    mStage->Render();

    mRenderer->Finalize();
}

} // namespace game
