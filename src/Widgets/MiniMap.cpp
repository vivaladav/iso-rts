#include "Widgets/MiniMap.h"

#include "GameConstants.h"
#include "Screens/ScreenGame.h"
#include "Widgets/GameUIData.h"

#include <sgl/core/event/MouseButtonEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ImageButton.h>

namespace game
{

class ButtonCloseMiniMap : public sgl::sgui::ImageButton
{
public:
    ButtonCloseMiniMap(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({
                                     IND_MINIMAP_BTN_CLOSE_NORMAL,
                                     IND_MINIMAP_BTN_CLOSE_NORMAL,
                                     IND_MINIMAP_BTN_CLOSE_OVER,
                                     IND_MINIMAP_BTN_CLOSE_PUSHED,
                                     IND_MINIMAP_BTN_CLOSE_NORMAL
                                 }, SpriteFileMapPanels, parent)
    {
    }
};

class ButtonMoveLeft : public sgl::sgui::ImageButton
{
public:
    ButtonMoveLeft(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({
                                     IND_MINIMAP_BTN_L_NORMAL,
                                     IND_MINIMAP_BTN_L_DISABLED,
                                     IND_MINIMAP_BTN_L_OVER,
                                     IND_MINIMAP_BTN_L_PUSHED,
                                     IND_MINIMAP_BTN_L_NORMAL
                                 }, SpriteFileMapPanels, parent)
    {
    }
};

class ButtonMoveRight : public sgl::sgui::ImageButton
{
public:
    ButtonMoveRight(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({
                                     IND_MINIMAP_BTN_R_NORMAL,
                                     IND_MINIMAP_BTN_R_DISABLED,
                                     IND_MINIMAP_BTN_R_OVER,
                                     IND_MINIMAP_BTN_R_PUSHED,
                                     IND_MINIMAP_BTN_R_NORMAL
                                 }, SpriteFileMapPanels, parent)
    {
    }
};

class ButtonMoveUp : public sgl::sgui::ImageButton
{
public:
    ButtonMoveUp(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({
                                     IND_MINIMAP_BTN_U_NORMAL,
                                     IND_MINIMAP_BTN_U_DISABLED,
                                     IND_MINIMAP_BTN_U_OVER,
                                     IND_MINIMAP_BTN_U_PUSHED,
                                     IND_MINIMAP_BTN_U_NORMAL
                                 }, SpriteFileMapPanels, parent)
    {
    }
};

class ButtonMoveDown : public sgl::sgui::ImageButton
{
public:
    ButtonMoveDown(sgl::sgui::Widget * parent)
        : sgl::sgui::ImageButton({
                                     IND_MINIMAP_BTN_D_NORMAL,
                                     IND_MINIMAP_BTN_D_DISABLED,
                                     IND_MINIMAP_BTN_D_OVER,
                                     IND_MINIMAP_BTN_D_PUSHED,
                                     IND_MINIMAP_BTN_D_NORMAL
                                 }, SpriteFileMapPanels, parent)
    {
    }
};

// ========== MINI MAP ==========

const int MAP_SCALE = 3;

MiniMap::MiniMap(int rows, int cols, ScreenGame * screen)
    : sgl::sgui::Widget(nullptr)
    , mScreen(screen)
    , mRows(rows)
    , mCols(cols)
{
    using namespace sgl;

    // init elements data
    mElementsMap.assign(mRows * mCols, nullptr);

    // BACKGROUND
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_BG);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(tex->GetWidth(), tex->GetHeight());

    // MAP AREA
    const int maxSize = 240;
    const int maxCells = (maxSize / MAP_SCALE) - 1;
    const int mapW0 = cols * MAP_SCALE;
    const int mapH0 = rows * MAP_SCALE;
    const bool mapBiggerThanW = mapW0 > maxSize;
    const bool mapBiggerThanH = mapH0 > maxSize;
    mMapW = mapBiggerThanW ? maxSize : mapW0;
    mMapH = mapBiggerThanH ? maxSize : mapH0;

    mR1 = mapBiggerThanW ? maxCells : (mMapH / MAP_SCALE) - 1;
    mC1 = mapBiggerThanH ? maxCells : (mMapW / MAP_SCALE) - 1;

    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_MAP_BG);
    mMapBg = new graphic::Image(tex);
    mMapBg->SetWidth(mMapW);
    mMapBg->SetHeight(mMapH);
    RegisterRenderable(mMapBg);

    // CAMERA CORNERS
    sgl::graphic::Camera * cam = GetCamera();
    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_CAMERA_TL);
    mCameraCornerTL = new graphic::Image(tex);
    mCameraCornerTL->SetCamera(cam);

    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_CAMERA_TR);
    mCameraCornerTR = new graphic::Image(tex);
    mCameraCornerTR->SetCamera(cam);

    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_CAMERA_BL);
    mCameraCornerBL = new graphic::Image(tex);
    mCameraCornerBL->SetCamera(cam);

    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_CAMERA_BR);
    mCameraCornerBR = new graphic::Image(tex);
    mCameraCornerBR->SetCamera(cam);

    // set camera move to 25% of map
    mMapMove = mRows * 0.25f;

    // BUTTON LEFT
    const int marginButtonsDir = 5;
    mButtonL = new ButtonMoveLeft(this);
    mButtonL->SetPosition(marginButtonsDir, (GetHeight() - mButtonL->GetHeight()) * 0.5f);

    mButtonL->AddOnClickFunction([this]
    {
        // update map columns
        int inc = mMapMove;

        if(mC0 - inc < 0)
            inc = mC0;

        mC0 -= inc;
        mC1 -= inc;

        // reposition elements
        const int incX = inc * MAP_SCALE;
        MoveContentHorizontal(incX);

        // update buttons
        UpdateAreaButtons();
    });

    // BUTTON UP
    mButtonU = new ButtonMoveUp(this);
    mButtonU->SetPosition((GetWidth() - mButtonU->GetWidth()) * 0.5f, marginButtonsDir);

    mButtonU->AddOnClickFunction([this]
    {
        // update map columns
        int inc = mMapMove;

        if(mR0 - inc < 0)
            inc = mR0;

        mR0 -= inc;
        mR1 -= inc;

        // reposition elements
        const int incY = inc * MAP_SCALE;
        MoveContentVertical(incY);

        // update buttons
        UpdateAreaButtons();
    });

    // BUTTON RIGHT
    mButtonR = new ButtonMoveRight(this);
    mButtonR->SetPosition(GetWidth() - marginButtonsDir - mButtonR->GetWidth(),
                          (GetHeight() - mButtonR->GetHeight()) * 0.5f);

    mButtonR->AddOnClickFunction([this]
    {
        // update map columns
        int inc = mMapMove;

        if(mC1 + inc >= mCols)
            inc = mCols - mC1 - 1;

        mC0 += inc;
        mC1 += inc;

        // reposition elements
        const int incX = -inc * MAP_SCALE;
        MoveContentHorizontal(incX);

        // update buttons
        UpdateAreaButtons();
    });

    // BUTTON DOWN
    mButtonD = new ButtonMoveDown(this);
    mButtonD->SetPosition((GetWidth() - mButtonD->GetWidth()) * 0.5f,
                          GetHeight() - marginButtonsDir - mButtonD->GetHeight());

    mButtonD->AddOnClickFunction([this]
    {
        // update map columns
        int inc = mMapMove;

        if(mR1 + inc >= mRows)
            inc = mRows - mR1 - 1;

        mR0 += inc;
        mR1 += inc;

        // reposition elements
        const int incY = -inc * MAP_SCALE;
        MoveContentVertical(incY);

        // update buttons
        UpdateAreaButtons();
    });

    UpdateAreaButtons();

    // CLOSE BUTTON
    const int marginButton = 5;

    mButtonClose = new ButtonCloseMiniMap(this);
    const int btnX = GetWidth() - mButtonClose->GetWidth() - marginButton;
    const int btnY = marginButton;
    mButtonClose->SetPosition(btnX, btnY);
}

MiniMap::~MiniMap()
{
    // delete elements
    for(auto elem : mElementsRenderingList)
        delete elem;

    // delete camera corners
    delete mCameraCornerTL;
    delete mCameraCornerTR;
    delete mCameraCornerBL;
    delete mCameraCornerBR;
}

void MiniMap::AddFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->AddOnClickFunction(f);
}

void MiniMap::AddElement(int r0, int c0, int rows, int cols, PlayerFaction faction)
{
    using namespace sgl;

    const int r1 = r0 - rows + 1;
    const int c1 = c0 - cols + 1;

    // check no cell is covered
    for(int r = r0; r >= r1; --r)
    {
        const int ind0 = r * mCols;

        for(int c = c0; c >= c1; --c)
        {
            const int ind = ind0 + c;

            // found existing element
            if(mElementsMap[ind])
                return ;
        }
    }

    // create and setup image
    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_MAP_ELEM);
    auto img = new sgl::graphic::Image(tex);
    img->SetCamera(GetCamera());
    img->SetWidth(cols * MAP_SCALE);
    img->SetHeight(rows * MAP_SCALE);

    const unsigned int color = faction != NO_FACTION ? PLAYER_COLOR[faction] : 0xA6A6A6FF;
    img->SetColor(color);

    // create and store element
    auto elem = new MiniMapElem(r0, c0, r1, c1, faction, img);

    PositionElement(elem);

    for(int r = r0; r >= r1; --r)
    {
        const int ind0 = r * mCols;

        for(int c = c0; c >= c1; --c)
        {
            const int ind = ind0 + c;

            mElementsMap[ind] = elem;
        }
    }

    mElementsRenderingList.push_back(elem);
}

void MiniMap::RemoveElement(int r0, int c0)
{
    const int ind0 = r0 * mCols + c0;
    MiniMapElem * elem = mElementsMap[ind0];

    // nothing here
    if(nullptr == elem)
        return ;

    // free cells occupied by elem
    for(int r = elem->brR; r >= elem->tlR; --r)
    {
        const int ind0 = r * mCols;

        for(int c = elem->brC; c >= elem->tlC; --c)
        {
            const int ind = ind0 + c;

            mElementsMap[ind] = nullptr;
        }
    }

    // remove from rendering
    auto it = mElementsRenderingList.begin();

    while(it != mElementsRenderingList.end())
    {
        if(*it == elem)
        {
            mElementsRenderingList.erase(it);
            break;
        }
        else
            ++it;
    }

    // delete element
    // TODO reuse elements
    delete elem;
}

void MiniMap::MoveElement(int startRow, int startCol, int endRow, int endCol)
{
    const int indElem = startRow * mCols + startCol;
    MiniMapElem * elem = mElementsMap[indElem];

    if(nullptr == elem)
        return ;

    // free cells occupied by elem
    for(int r = elem->brR; r >= elem->tlR; --r)
    {
        const int ind0 = r * mCols;

        for(int c = elem->brC; c >= elem->tlC; --c)
        {
            const int ind = ind0 + c;

            mElementsMap[ind] = nullptr;
        }
    }

    // store elem in new cells
    elem->brR = endRow;
    elem->brC = endCol;
    elem->tlR = endRow - elem->rows + 1;
    elem->tlC = endCol - elem->cols + 1;

    for(int r = elem->brR; r >= elem->tlR; --r)
    {
        const int ind0 = r * mCols;

        for(int c = elem->brC; c >= elem->tlC; --c)
        {
            const int ind = ind0 + c;

            mElementsMap[ind] = elem;
        }
    }

    // move image
    PositionElement(elem);
}

void MiniMap::SetCameraCells(const Cell2D & tl, const Cell2D & tr, const Cell2D & bl, const Cell2D & br)
{
    const int x0 = mMapX - mC0 * MAP_SCALE;
    const int y0 = mMapY - mR0 * MAP_SCALE;

    // iso TL -> mm BL
    const int blX = x0 + tl.col * MAP_SCALE + MAP_SCALE * 0.5f;
    const int blY = y0 + tl.row * MAP_SCALE + (MAP_SCALE - mCameraCornerBL->GetHeight()) * 0.5f;
    mCameraCornerBL->SetPosition(blX, blY);

    // iso BL -> mm BR
    const int brX = x0 + bl.col * MAP_SCALE + (MAP_SCALE - mCameraCornerBR->GetWidth()) * 0.5f;
    const int brY = y0 + bl.row * MAP_SCALE + MAP_SCALE * 0.5f - mCameraCornerBR->GetHeight();
    mCameraCornerBR->SetPosition(brX, brY);

    // iso BR -> mm TR
    const int trX = x0 + br.col * MAP_SCALE + MAP_SCALE * 0.5f - mCameraCornerTR->GetWidth();
    const int trY = y0 + br.row * MAP_SCALE + (MAP_SCALE - mCameraCornerTR->GetHeight()) * 0.5f;
    mCameraCornerTR->SetPosition(trX, trY);

    // iso TR -> mm TL
    const int tlX = x0 + tr.col * MAP_SCALE + (MAP_SCALE - mCameraCornerTL->GetWidth()) * 0.5f;
    const int tlY = y0 + tr.row * MAP_SCALE + MAP_SCALE * 0.5f;
    mCameraCornerTL->SetPosition(tlX, tlY);
}

void MiniMap::PositionElement(MiniMapElem * elem)
{
    const int imgX = mMapX + (elem->tlC - mC0) * MAP_SCALE;
    const int imgY = mMapY + (elem->tlR - mR0) * MAP_SCALE;
    elem->img->SetPosition(imgX, imgY);
}

void MiniMap::MoveContentHorizontal(int val)
{
    for(auto elem : mElementsRenderingList)
    {
        sgl::graphic::Image * img = elem->img;
        img->SetX(img->GetX() + val);
    }

    // reposition camera view
    mCameraCornerBL->SetX(mCameraCornerBL->GetX() + val);
    mCameraCornerBR->SetX(mCameraCornerBR->GetX() + val);
    mCameraCornerTL->SetX(mCameraCornerTL->GetX() + val);
    mCameraCornerTR->SetX(mCameraCornerTR->GetX() + val);
}

void MiniMap::MoveContentVertical(int val)
{
    for(auto elem : mElementsRenderingList)
    {
        sgl::graphic::Image * img = elem->img;
        img->SetY(img->GetY() + val);
    }

    // reposition camera view
    mCameraCornerBL->SetY(mCameraCornerBL->GetY() + val);
    mCameraCornerBR->SetY(mCameraCornerBR->GetY() + val);
    mCameraCornerTL->SetY(mCameraCornerTL->GetY() + val);
    mCameraCornerTR->SetY(mCameraCornerTR->GetY() + val);
}

void MiniMap::UpdateAreaButtons()
{
    mButtonU->SetEnabled(mR0 > 0);
    mButtonD->SetEnabled(mR1 < (mRows - 1));
    mButtonL->SetEnabled(mC0 > 0);
    mButtonR->SetEnabled(mC1 < (mCols - 1));
}

void MiniMap::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    // BACKGROUND
    mBg->SetPosition(x0 ,y0);

    // MAP
    mMapX = x0 + (GetWidth() - mMapBg->GetWidth()) * 0.5f;
    mMapY = y0 + (GetHeight() - mMapBg->GetHeight()) * 0.5f;
    mMapBg->SetPosition(mMapX, mMapY);
}

void MiniMap::HandleMouseButtonDown(sgl::core::MouseButtonEvent & event)
{
    event.SetConsumed();
}

void MiniMap::HandleMouseButtonUp(sgl::core::MouseButtonEvent & event)
{
    event.SetConsumed();

    const int x = event.GetX();
    const int y = event.GetY();

    // click is within the map area
    if(x > mMapX && x < (mMapX + mMapW) && y > mMapY &&  y < (mMapY + mMapH))
    {
        const int col = mC0 + (x - mMapX) / MAP_SCALE;
        const int row = mR0 + (y - mMapY) / MAP_SCALE;

        mScreen->CenterCameraOverCell(row, col);
    }
}

void MiniMap::HandleMouseMotion(sgl::core::MouseMotionEvent & event)
{
    event.SetConsumed();
}

void MiniMap::OnRender()
{
    sgl::sgui::Widget::OnRender();

    auto renderer = sgl::graphic::Renderer::Instance();

    renderer->SetClipping(mMapX, mMapY, mMapW, mMapH);

    // map elements
    for(auto elem : mElementsRenderingList)
        elem->img->Render();

    // camera corners
    mCameraCornerTL->Render();
    mCameraCornerTR->Render();
    mCameraCornerBL->Render();
    mCameraCornerBR->Render();

    renderer->ClearClipping();
}

} // namespace game
