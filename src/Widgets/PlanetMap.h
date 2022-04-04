#include <sgl/sgui/Widget.h>


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

private:
    void HandlePositionChanged() override;

private:
    static const unsigned int MAX_MISSIONS = 12;

    sgl::sgui::AbstractButtonsGroup * mButtonMission;

    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
