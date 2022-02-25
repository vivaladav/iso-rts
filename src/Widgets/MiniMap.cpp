#include "Widgets/MiniMap.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ImageButton.h>

namespace game
{

const std::array<unsigned int, sgl::sgui::PushButton::NUM_VISUAL_STATES> texIds =
{
    IND_MINIMAP_BTN_CLOSE_NORMAL,
    IND_MINIMAP_BTN_CLOSE_NORMAL,
    IND_MINIMAP_BTN_CLOSE_OVER,
    IND_MINIMAP_BTN_CLOSE_PUSHED,
    IND_MINIMAP_BTN_CLOSE_NORMAL
};

class ButtonCloseMiniMap : public sgl::sgui::ImageButton
{
public:
    ButtonCloseMiniMap(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton(texIds, SpriteFileMapPanels, parent)
    {
    }
};

MiniMap::MiniMap()
    : sgl::sgui::Widget(nullptr)
{
    using namespace sgl;

    // BACKGROUND
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_BG);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(tex->GetWidth(), tex->GetHeight());

    // CLOSE BUTTON
    const int marginButton = 5;

    mButtonClose = new ButtonCloseMiniMap(this);
    const int btnX = GetWidth() - mButtonClose->GetWidth() - marginButton;
    const int btnY = marginButton;
    mButtonClose->SetPosition(btnX, btnY);
}

void MiniMap::AddFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

void MiniMap::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBg->SetPosition(x0 ,y0);
}

} // namespace game
