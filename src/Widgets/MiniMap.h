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

struct MiniMapElem
{
    MiniMapElem(int r, int c, int rs, int cs,
                PlayerFaction f, sgl::graphic::Image * i)
        : r0(r)
        , c0(c)
        , rows(rs)
        , cols(cs)
        , faction(f)
        , img(i)
    {}

    int r0;
    int c0;
    int rows;
    int cols;
    PlayerFaction faction;
    sgl::graphic::Image * img;
};

class MiniMap : public sgl::sgui::Widget
{
public:
    MiniMap(int rows, int cols);

    void AddFunctionOnClose(const std::function<void()> & f);

    void AddElement(int r0, int c0, int rows, int cols, PlayerFaction faction);
    void MoveElement(int startRow, int startCol, int endRow, int endCol);

private:
    void PositionImage(sgl::graphic::Image * elem, int tlRow, int tlCol);

    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::vector<MiniMapElem *> mElementsMap;
    std::vector<MiniMapElem *> mElementsRenderingList;

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
