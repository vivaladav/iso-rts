#include "Screens/ScreenPlanetMap.h"

#include "Game.h"
#include "States/StatesIds.h"
#include "Widgets/PlanetMap.h"

#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/Stage.h>

namespace game
{

ScreenPlanetMap::ScreenPlanetMap(Game * game)
    : Screen(game)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();
    graphic::Texture * tex = nullptr;
    graphic::Font * fnt = nullptr;

    // BACKGROUND
    tex = tm->GetTexture("data/img/space_bg.jpg");
    mBg = new graphic::Image(tex);

    // PLANET
    mPlanet = new PlanetMap;

    const int planetX = (mBg->GetWidth() - mPlanet->GetWidth()) * 0.5f;
    const int planetY = (mBg->GetHeight() - mPlanet->GetHeight()) * 0.5f;
    mPlanet->SetPosition(planetX, planetY);
}

ScreenPlanetMap::~ScreenPlanetMap()
{
    sgl::sgui::Stage::Instance()->ClearWidgets();

    delete mBg;
}

void ScreenPlanetMap::Update(float update)
{
}

void ScreenPlanetMap::Render()
{
    mBg->Render();
}

} // namespace game
