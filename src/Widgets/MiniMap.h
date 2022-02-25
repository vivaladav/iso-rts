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
    MiniMap(int rows, int cols);

    void AddFunctionOnClose(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Image * mMapBg = nullptr;

    ButtonCloseMiniMap * mButtonClose = nullptr;

    int mRows = 0;
    int mCols = 0;
};

} // namespace game
