#include <sgl/sgui/Widget.h>

#include "Cell2D.h"

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
    MiniMapElem(int r0, int c0, int r1, int c1,
                PlayerFaction f, sgl::graphic::Image * i)
        : brR(r0)
        , brC(c0)
        , tlR(r1)
        , tlC(c1)
        , rows(r0 - r1 + 1)
        , cols(c0 - c1 + 1)
        , faction(f)
        , img(i)
    {}

    int brR;
    int brC;
    int tlR;
    int tlC;
    int rows;
    int cols;
    PlayerFaction faction;
    sgl::graphic::Image * img;
};

class MiniMap : public sgl::sgui::Widget
{
public:
    MiniMap(int rows, int cols);
    ~MiniMap();

    void AddFunctionOnClose(const std::function<void()> & f);

    void AddElement(int r0, int c0, int rows, int cols, PlayerFaction faction);
    void RemoveElement(int r0, int c0);
    void MoveElement(int startRow, int startCol, int endRow, int endCol);

    void SetCameraCells(const Cell2D & tl, const Cell2D & tr, const Cell2D & bl, const Cell2D & br);

private:
    void PositionElement(MiniMapElem * elem);

    void MoveContentHorizontal(int val);
    void MoveContentVertical(int val);

    void UpdateAreaButtons();

    void HandlePositionChanged() override;

    void HandleMouseButtonDown(sgl::core::MouseButtonEvent & event) override;
    void HandleMouseButtonUp(sgl::core::MouseButtonEvent & event) override;
    void HandleMouseMotion(sgl::core::MouseMotionEvent & event) override;

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

    sgl::graphic::Image * mCameraCornerTL = nullptr;
    sgl::graphic::Image * mCameraCornerTR = nullptr;
    sgl::graphic::Image * mCameraCornerBL = nullptr;
    sgl::graphic::Image * mCameraCornerBR = nullptr;


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

    int mMapMove = 0;
};

} // namespace game
