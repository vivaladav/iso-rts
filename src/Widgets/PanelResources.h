#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>
#include <string>
#include <vector>

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
    ~PanelResources();

private:
    void HandlePositionChanged() override;

    void SetBg();

private:
    sgl::graphic::Image * mBg = nullptr;

    Player * mPlayer = nullptr;

    std::vector<unsigned int> mCallbackValIds;
    std::vector<unsigned int> mCallbackRangeIds;
};

} // namespace game
