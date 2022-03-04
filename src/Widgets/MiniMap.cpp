#include "Widgets/MiniMap.h"

#include "GameConstants.h"
#include "Widgets/GameUIData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/sgui/ImageButton.h>

#include <iostream>

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

MiniMap::MiniMap(int rows, int cols)
    : sgl::sgui::Widget(nullptr)
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
    const int maxSize = 204;
    const int maxCells = maxSize / MAP_SCALE;
    const int mapW0 = cols * MAP_SCALE;
    const int mapH0 = rows * MAP_SCALE;
    const bool mapBiggerThanW = mapW0 > maxSize;
    const bool mapBiggerThanH = mapH0 > maxSize;
    mMapW = mapBiggerThanW ? maxSize : mapW0;
    mMapH = mapBiggerThanH ? maxSize : mapH0;

    mR1 = mapBiggerThanW ? maxCells : mMapH / MAP_SCALE;
    mC1 = mapBiggerThanH ? maxCells : mMapW / MAP_SCALE;

    tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_MAP_BG);
    mMapBg = new graphic::Image(tex);
    mMapBg->SetWidth(mMapW);
    mMapBg->SetHeight(mMapH);
    RegisterRenderable(mMapBg);

    std::cout << "MINIMAP - TL: " << mR0 << "," << mC0 << " - BR: " << mR1 << "," << mC1
              << " - MAP: " << rows << "x" << cols
              << " - SIZE: " << mMapW << "x" << mMapH << std::endl;

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

    // BUTTON LEFT
    const int marginButtonsDir = 6;
    mButtonL = new ButtonMoveLeft(this);
    mButtonL->SetPosition(marginButtonsDir, (GetHeight() - mButtonL->GetHeight()) * 0.5f);

    // BUTTON UP
    mButtonU = new ButtonMoveUp(this);
    mButtonU->SetPosition((GetWidth() - mButtonU->GetWidth()) * 0.5f, marginButtonsDir);

    // BUTTON RIGHT
    mButtonR = new ButtonMoveRight(this);
    mButtonR->SetPosition(GetWidth() - marginButtonsDir - mButtonR->GetWidth(),
                          (GetHeight() - mButtonR->GetHeight()) * 0.5f);

    // BUTTON DOWN
    mButtonD= new ButtonMoveDown(this);
    mButtonD->SetPosition((GetWidth() - mButtonD->GetWidth()) * 0.5f,
                          GetHeight() - marginButtonsDir - mButtonD->GetHeight());

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
    // iso TL -> mm BL
    const int blX = mMapX + tl.col * MAP_SCALE + MAP_SCALE * 0.5f;
    const int blY = mMapY + tl.row * MAP_SCALE + (MAP_SCALE - mCameraCornerBL->GetHeight()) * 0.5f;
    mCameraCornerBL->SetPosition(blX, blY);

    // iso BL -> mm BR
    const int brX = mMapX + bl.col * MAP_SCALE + (MAP_SCALE - mCameraCornerBR->GetWidth()) * 0.5f;
    const int brY = mMapY + bl.row * MAP_SCALE + MAP_SCALE * 0.5f - mCameraCornerBR->GetHeight();
    mCameraCornerBR->SetPosition(brX, brY);

    // iso BR -> mm TR
    const int trX = mMapX + br.col * MAP_SCALE + MAP_SCALE * 0.5f - mCameraCornerTR->GetWidth();
    const int trY = mMapY + br.row * MAP_SCALE + (MAP_SCALE - mCameraCornerTR->GetHeight()) * 0.5f;
    mCameraCornerTR->SetPosition(trX, trY);

    // iso TR -> mm TL
    const int tlX = mMapX + tr.col * MAP_SCALE + (MAP_SCALE - mCameraCornerTL->GetWidth()) * 0.5f;
    const int tlY = mMapY + tr.row * MAP_SCALE + MAP_SCALE * 0.5f;
    mCameraCornerTL->SetPosition(tlX, tlY);
}

void MiniMap::PositionElement(MiniMapElem * elem)
{
    const int imgX = mMapX + (elem->tlC - mC0) * MAP_SCALE;
    const int imgY = mMapY + (elem->tlR - mR0) * MAP_SCALE;
    elem->img->SetPosition(imgX, imgY);
}

void MiniMap::UpdateAreaButtons()
{
    mButtonU->SetEnabled(mR0 > 0);
    mButtonD->SetEnabled(mR1 < mRows);
    mButtonL->SetEnabled(mC0 > 0);
    mButtonR->SetEnabled(mC1 < mCols);
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
