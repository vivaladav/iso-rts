#include "Widgets/PlanetMap.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

PlanetMap::PlanetMap()
    : sgl::sgui::Widget(nullptr)
{
    auto tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = nullptr;

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_PLANET);
    mBg = new sgl::graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(mBg->GetWidth(), mBg->GetHeight());
}

void PlanetMap::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBg->SetPosition(x0, y0);
}

} // namespace game
