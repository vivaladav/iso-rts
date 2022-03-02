#include "Widgets/MiniMap.h"

#include "GameConstants.h"
#include "Widgets/GameUIData.h"

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

    // BUTTON LEFT
    const int marginButtonsDir = 6;
    mButtonL = new ButtonMoveLeft(this);
    mButtonL->SetEnabled(false);
    mButtonL->SetPosition(marginButtonsDir, (GetHeight() - mButtonL->GetHeight()) * 0.5f);

    // BUTTON UP
    mButtonU = new ButtonMoveUp(this);
    mButtonU->SetEnabled(false);
    mButtonU->SetPosition((GetWidth() - mButtonU->GetWidth()) * 0.5f, marginButtonsDir);

    // BUTTON RIGHT
    mButtonR = new ButtonMoveRight(this);
    mButtonR->SetEnabled(false);
    mButtonR->SetPosition(GetWidth() - marginButtonsDir - mButtonR->GetWidth(),
                          (GetHeight() - mButtonR->GetHeight()) * 0.5f);

    // BUTTON DOWN
    mButtonD= new ButtonMoveDown(this);
    mButtonD->SetEnabled(false);
    mButtonD->SetPosition((GetWidth() - mButtonD->GetWidth()) * 0.5f,
                          GetHeight() - marginButtonsDir - mButtonD->GetHeight());

    // CLOSE BUTTON
    const int marginButton = 5;

    mButtonClose = new ButtonCloseMiniMap(this);
    const int btnX = GetWidth() - mButtonClose->GetWidth() - marginButton;
    const int btnY = marginButton;
    mButtonClose->SetPosition(btnX, btnY);
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

    auto tm = graphic::TextureManager::Instance();
    auto tex = tm->GetSprite(SpriteFileMapPanels, IND_MINIMAP_MAP_ELEM);
    auto img = new sgl::graphic::Image(tex);
    img->SetCamera(GetCamera());

    const int imgX = mMapX + c1 * MAP_SCALE;
    const int imgY = mMapY + r1 * MAP_SCALE;
    img->SetPosition(imgX, imgY);

    img->SetWidth(cols * MAP_SCALE);
    img->SetHeight(rows * MAP_SCALE);

    const unsigned int color = faction != NO_FACTION ? PLAYER_COLOR[faction] : 0xA6A6A6FF;
    img->SetColor(color);

    for(int r = r0; r >= r1; --r)
    {
        const int ind0 = r * mCols;

        for(int c = c0; c >= c1; --c)
        {
            const int ind = ind0 + c;

            mElementsMap[ind] = img;
        }
    }

    mElementsRenderingList.push_back(img);
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

    for(auto elem : mElementsRenderingList)
        elem->Render();

    renderer->ClearClipping();
}

} // namespace game
