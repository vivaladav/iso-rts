#pragma once

#include <sgui/Widget.h>

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

class PanelResourcesBar : public lib::sgui::Widget
{
public:
    PanelResourcesBar(Player * player);
    ~PanelResourcesBar();

private:
    void OnRender() override;

    void HandlePositionChanged() override;

    void SetBg();

private:
    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
