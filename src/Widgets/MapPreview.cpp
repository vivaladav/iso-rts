#include "MapPreview.h"

#include "IsoMap.h"

#include "graphic/Image.h"

#include <string>
#include <vector>

namespace game
{

MapPreview::MapPreview(const char * file, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
    // -- BACKGROUND --
    mBg = new lib::graphic::Image("data/img/map_preview_bg.png");
    SetSize(mBg->GetWidth(), mBg->GetHeight());


    // -- ISOMETRIC MAP --
    const int TILE_W = 28;

    const std::vector<std::string> tiles =
    {
        // scene
        "data/img/tile_prev-00.png",
        "data/img/tile_prev-01.png"
    };

    mIsoMap = new IsoMap(file, TILE_W);
    mIsoMap->SetTiles(tiles);
}

bool MapPreview::Load(const char * file)
{
    // load file
    if(!mIsoMap->Load(file))
        return false;

    // reposition map
    HandlePositionChanged();

    return true;
}

void MapPreview::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mBg->SetPosition(x0, y0);

    const int x = x0 + mBg->GetWidth() * 0.5f;
    const int y = y0 + (mBg->GetHeight() - mIsoMap->GetHeight()) * 0.5f;

    mIsoMap->SetOrigin(x, y);
}

void MapPreview::OnRender()
{
    mBg->Render();
    mIsoMap->Render();
}

} // namespace game
