#include "MapPreview.h"

#include "IsoMap.h"
#include "MapLoader.h"

#include <sgl/core/event/MouseButtonEvent.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Text.h>

#include <sstream>

namespace game
{

MapPreview::MapPreview(const std::string & file, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
    , mOnClick([]{})
{
    using namespace sgl::graphic;

    // -- BACKGROUND --
    mBgUnsel = new Image("data/img/map_preview_bg.png");
    mBgSel = new Image("data/img/map_preview_sel_bg.png");
    mBg = mBgUnsel;
    SetSize(mBg->GetWidth(), mBg->GetHeight());

    // -- ISOMETRIC MAP --
    const int TILE_W = 28;

    const std::vector<std::string> tiles =
    {
        // scene
        "data/img/tile_prev-00.png",
        "data/img/tile_prev-01.png"
    };

    mIsoMap = new IsoMap(TILE_W);
    mIsoMap->SetTiles(tiles);

    MapLoader ml(nullptr, mIsoMap);
    ml.Load(file);

    // -- LABEL SIZE --
    std::ostringstream os;
    os << mIsoMap->GetNumRows() << "x" << mIsoMap->GetNumCols() << std::ends;;

    Font * font = FontManager::Instance()->GetFont("data/fonts/OpenSans.ttf", 24, Font::NORMAL);

    mLabelSize = new Text(os.str().c_str(), font);
    mLabelSize->SetColor(0x888888FF);
}

void MapPreview::SetSelected(bool val)
{
    if(val)
        mBg = mBgSel;
    else
        mBg = mBgUnsel;
}

void MapPreview::HandleMouseButtonUp(sgl::core::MouseButtonEvent & event)
{
    event.SetConsumed();

    mOnClick();
}

void MapPreview::HandlePositionChanged()
{
    // background
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBgUnsel->SetPosition(x0, y0);
    mBgSel->SetPosition(x0, y0);

    // map
    const int x = x0 + mBg->GetWidth() * 0.5f;
    const int y = y0 + (mBg->GetHeight() - mIsoMap->GetHeight()) * 0.5f;

    mIsoMap->SetOrigin(x, y);

    // label
    const int marginLabelT = 0;

    const int x1 = x0 + (mBg->GetWidth() - mLabelSize->GetWidth()) * 0.5f;
    const int y1 = y0 + mBg->GetHeight() + marginLabelT;

    mLabelSize->SetPosition(x1, y1);
}

void MapPreview::OnRender()
{
    mBg->Render();

    auto * renderer = sgl::graphic::Renderer::Instance();

    const int x0 = mBg->GetX() + 1;
    const int y0 = mBg->GetY() + 1;
    const int w = mBg->GetWidth() - 2;
    const int h = mBg->GetHeight() - 2;
    renderer->SetClipping(x0, y0, w, h);
    mIsoMap->Render();
    renderer->ClearClipping();

    mLabelSize->Render();
}

} // namespace game
