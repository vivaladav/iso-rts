#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

enum PlayerFaction : unsigned int;

class PlanetMap : public sgl::sgui::Widget
{
public:
    PlanetMap();

private:
    void HandlePositionChanged() override;

private:
    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
