#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class ImageButton; }
}

namespace game
{

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

    sgl::sgui::ImageButton * mButtonClose = nullptr;

    sgl::sgui::ImageButton * mButtonL = nullptr;
    sgl::sgui::ImageButton * mButtonR = nullptr;
    sgl::sgui::ImageButton * mButtonU = nullptr;
    sgl::sgui::ImageButton * mButtonD = nullptr;

    int mRows = 0;
    int mCols = 0;
};

} // namespace game
