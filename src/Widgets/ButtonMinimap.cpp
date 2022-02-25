#include "Widgets/ButtonMinimap.h"

#include "Widgets/GameUIData.h"

namespace game

{

const std::array<unsigned int, sgl::sgui::PushButton::NUM_VISUAL_STATES> texIds =
{
    IND_MINIMAP_BUTTON_NORMAL,
    IND_MINIMAP_BUTTON_NORMAL,
    IND_MINIMAP_BUTTON_OVER,
    IND_MINIMAP_BUTTON_PUSHED,
    IND_MINIMAP_BUTTON_NORMAL
};

ButtonMinimap::ButtonMinimap()
    : sgl::sgui::ImageButton(texIds, SpriteFileMapPanels, nullptr)
{
}

} // namespace game
