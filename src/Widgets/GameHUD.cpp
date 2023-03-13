#include "GameHUD.h"

#include <sgl/graphic/Renderer.h>

namespace game
{

GameHUD::GameHUD()
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    SetSize(rendW, rendH);
}

} // namespace game
