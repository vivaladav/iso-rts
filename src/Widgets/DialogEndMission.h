#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class AbstractButton; }
}

namespace game
{

class Game;
class Screen;

class DialogEndMission : public sgl::sgui::Widget
{
public:
    DialogEndMission(int time, int territoryConquered, int enemiesKilled,
                     int casualties, bool victory);

    void SetFunctionOnClose(const std::function<void()> & f);

protected:
    void HandlePositionChanged() override;

private:
    void SetPositions();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::sgui::AbstractButton * mButton = nullptr;
};

} // namespace game
