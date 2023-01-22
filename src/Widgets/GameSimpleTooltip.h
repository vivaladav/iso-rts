#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class Game;

class GameSimpleTooltip : public sgl::sgui::Widget
{
public:
    GameSimpleTooltip(const char * text);

protected:
    void HandlePositionChanged() override;

private:
    void SetPositions();

private:
    sgl::graphic::Image * mBgLeft = nullptr;
    sgl::graphic::Image * mBgCont = nullptr;
    sgl::graphic::Image * mBgRight = nullptr;
    sgl::graphic::Text * mLabel = nullptr;
};

} // namespace game
