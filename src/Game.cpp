#include "Game.h"

#include "GameConstants.h"
#include "MapsRegistry.h"
#include "Player.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "States/StatesIds.h"
#include "States/StateFactionSelection.h"
#include "States/StateGame.h"
#include "States/StateInit.h"
#include "States/StateMainMenu.h"
#include "States/StateNewGame.h"
#include "States/StatePlanetMap.h"
#include "States/StateSettings.h"
#include "States/StateTest.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/GraphicConstants.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/graphic/Window.h>
#include <sgl/sgui/Stage.h>
#include <sgl/utilities/StateManager.h>

#include <iostream>

namespace game
{

Game::Game(int argc, char * argv[])
    : sgl::core::Application(argc, argv)
    , mMapsReg(new MapsRegistry)
    , mObjsRegistry(new ObjectsDataRegistry)
    , mLocalFaction(NO_FACTION)
    , mCurrPlanet(PLANET_UNKNOWN)
{
    using namespace sgl::graphic;

    mWin = Window::Create("Virtueror - v. 0.0.1", 1600, 900);
    mRenderer = Renderer::Create(mWin);
    mRenderer->SetLogicalSize(1920, 1080);

    TextureManager::Instance()->SetNewTextureQuality(TextureQuality::BEST);

    // -- State Manager --
    mStateMan = new sgl::utilities::StateManager;

    mStateMan->AddState(new StateFactionSelection(this));
    mStateMan->AddState(new StateGame(this));
    mStateMan->AddState(new StateInit(this));
    mStateMan->AddState(new StateMainMenu(this));
    mStateMan->AddState(new StateNewGame(this));
    mStateMan->AddState(new StatePlanetMap(this));
    mStateMan->AddState(new StateSettings(this));
    mStateMan->AddState(new StateTest(this));

    mStateMan->RequestNextActiveState(StateId::INIT);

    // -- SGUI Stage --
    mStage = sgl::sgui::Stage::Create();
    AddKeyboardListener(mStage);
    AddMouseListener(mStage);

    FontManager * fm = FontManager::Create();
    mFontGui = fm->GetFont("data/fonts/OpenSans.ttf", 32, Font::BOLD);
    mStage->SetDefaultFont(mFontGui);

    // -- MAPS --
    // PLANET 1
    mMapsReg->CreatePlanet(PLANET_1);
    //               planetId, file, energy, material, diamonds, blobs, size, value, occupier, status
    mMapsReg->AddMap(PLANET_1, "data/maps/80x80-01.map", 5, 4, 2, 2, 80, 3, NO_FACTION, TER_ST_UNEXPLORED);
    mMapsReg->AddMap(PLANET_1, "data/maps/40x40-01.map", 2, 2, 1, 1, 40, 2, NO_FACTION, TER_ST_UNEXPLORED);
    mMapsReg->AddMap(PLANET_1, "data/maps/60x60-01.map", 4, 4, 2, 2, 60, 3, NO_FACTION, TER_ST_UNREACHABLE);
    mMapsReg->AddMap(PLANET_1, "data/maps/20x20-empty.map", 1, 2, 1, 1, 20, 1, NO_FACTION, TER_ST_UNREACHABLE);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
    mMapsReg->AddUnavailableMap(PLANET_1);
}

Game::~Game()
{
    delete mMapsReg;
    delete mObjsRegistry;

    ClearPlayers();

    delete mStateMan;

    sgl::sgui::Stage::Destroy();

    sgl::graphic::FontManager::Destroy();

    sgl::graphic::Renderer::Destroy();
    sgl::graphic::Window::Destroy();
}

const std::string & Game::GetCurrentMapFile() const
{
    return mMapsReg->GetMapFile(mCurrPlanet, mCurrTerritory);
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


Player * Game::GetPlayerByFaction(PlayerFaction faction) const
{
    for(Player * p : mPlayers)
    {
        if(p->GetFaction() == faction)
            return p;
    }

    return nullptr;
}

} // namespace game
