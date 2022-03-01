#include <sgl/sgui/Widget.h>

#include <functional>
#include <vector>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class ImageButton; }
}

namespace game
{

enum PlayerFaction : unsigned int;

class MiniMap : public sgl::sgui::Widget
{
public:
    MiniMap(int rows, int cols);

    void AddFunctionOnClose(const std::function<void()> & f);

    void AddElement(int r0, int c0, int rows, int cols, PlayerFaction faction);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::vector<sgl::graphic::Image *> mElementsMap;
    std::vector<sgl::graphic::Image *> mElementsRenderingList;

    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Image * mMapBg = nullptr;

    sgl::sgui::ImageButton * mButtonClose = nullptr;

    sgl::sgui::ImageButton * mButtonL = nullptr;
    sgl::sgui::ImageButton * mButtonR = nullptr;
    sgl::sgui::ImageButton * mButtonU = nullptr;
    sgl::sgui::ImageButton * mButtonD = nullptr;

    int mRows = 0;
    int mCols = 0;

    int mR0 = 0;
    int mC0 = 0;
    int mR1 = 0;
    int mC1 = 0;

    int mMapX = 0;
    int mMapY = 0;
    int mMapW = 0;
    int mMapH = 0;
};

} // namespace game
