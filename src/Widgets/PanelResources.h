#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>
#include <string>

namespace lib
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class Player;

class PanelResources : public lib::sgui::Widget
{
public:
    PanelResources(Player * player);

private:
    void HandlePositionChanged() override;

    void SetBg();

private:
    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
