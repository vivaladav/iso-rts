#include "Screens/ScreenFactionSelection.h"

#include "Game.h"
#include "Player.h"
#include "States/StatesIds.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Renderer.h>
#include <sgui/Label.h>
#include <sgui/Stage.h>

#include <iostream>
#include <string>

namespace game
{

ScreenFactionSelection::ScreenFactionSelection(Game * game)
    : Screen(game)
    , mFaction(NO_FACTION)
{
    using namespace lib::graphic;
    using namespace lib::sgui;

    game->SetClearColor(0x0F, 0x0F, 0x0F, 0x0F);

}

ScreenFactionSelection::~ScreenFactionSelection()
{
    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenFactionSelection::Update(float update)
{
}

void ScreenFactionSelection::Render()
{
}

} // namespace game
