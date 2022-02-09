#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>
#include <string>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class Player;

class PanelResources : public sgl::sgui::Widget
{
public:
    PanelResources(Player * player);

private:
    void HandlePositionChanged() override;

    void SetBg();

private:
    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
