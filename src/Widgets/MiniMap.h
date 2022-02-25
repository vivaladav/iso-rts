#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class ButtonCloseMiniMap;

class MiniMap : public sgl::sgui::Widget
{
public:
    MiniMap();

    void AddFunctionOnClose(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

private:
    sgl::graphic::Image * mBg = nullptr;

    ButtonCloseMiniMap * mButtonClose = nullptr;
};

} // namespace game
