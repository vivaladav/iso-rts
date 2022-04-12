#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class AbstractButtonsGroup; }
}

namespace game
{

class ButtonMission;

enum PlayerFaction : unsigned int;

class PlanetMap : public sgl::sgui::Widget
{
public:
    PlanetMap();

    void SetFunctionOnToggle(const std::function<void(unsigned int, bool)> & f);

private:
    void HandlePositionChanged() override;

private:
    static const unsigned int MAX_MISSIONS = 12;

    sgl::sgui::AbstractButtonsGroup * mButtonsMission;

    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
