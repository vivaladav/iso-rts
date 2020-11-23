#include "MapPreview.h"

#include "IsoMap.h"

#include "graphic/Image.h"

#include <fstream>
#include <string>
#include <vector>

namespace game
{

MapPreview::MapPreview(lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
    // -- BACKGROUND --
    mBg = new lib::graphic::Image("data/img/map_preview_bg.png");
    SetSize(mBg->GetWidth(), mBg->GetHeight());


    // -- ISOMETRIC MAP --
    const int SIDE = 15;
    const int TILE_W = 28;

    const std::vector<std::string> tileFiles =
    {
        // scene
        "data/img/tile_prev-00.png",
        "data/img/tile_prev-01.png"
    };

    mIsoMap = new IsoMap(SIDE, SIDE, TILE_W);
    mIsoMap->SetTiles(tileFiles);
}

bool MapPreview::Load(const char * file)
{
    std::fstream f(file);

    if(!f.is_open())
        return false;

    std::string line;

    const unsigned int rows = mIsoMap->GetNumRows();
    const unsigned int cols = mIsoMap->GetNumCols();

    for(unsigned int r = 0; r < rows; ++r)
    {
        std::getline(f, line);

        for(unsigned int c = 0; c < cols; ++c)
        {
            const int type = line[c] - '0';

            mIsoMap->SetCellType(r, c, type);
        }
    }

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
