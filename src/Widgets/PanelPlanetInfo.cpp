#include "Widgets/PanelPlanetInfo.h"

#include "GameConstants.h"
#include "Widgets/GameUIData.h"
#include "Widgets/WidgetsConstants.h"

#include <sgl/core/Point.h>
#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Text.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

#include <sgl/sgui/Image.h>

#include <sstream>

namespace game
{

PanelPlanetInfo::PanelPlanetInfo()
    : sgl::sgui::Widget(nullptr)
    , mOccupier(NO_FACTION)
    , mStatus(TER_ST_UNKNOWN)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    auto fm = graphic::FontManager::Instance();

    // BACKGROUND
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap2, IND_PM_PANEL_INFO);
    mBg = new graphic::Image(tex);
    RegisterRenderable(mBg);

    SetSize(tex->GetWidth(), tex->GetHeight());

    // TITLE
    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    const unsigned int colorTitle = 0xe9f7fbcc;

    graphic::Font * fnt = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapTitle,
                                      graphic::Font::NORMAL);
    mTitle = new graphic::Text("INFO", fnt);
    mTitle->SetColor(colorTitle);
    RegisterRenderable(mTitle);

    // -- DATA --
    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    // LINE SIZE
    mHeaderSize = new graphic::Text("SIZE", fntData);
    mHeaderSize->SetColor(WidgetsConstants::colorPlanetMapHeader);
    RegisterRenderable(mHeaderSize);

    mLabelSize = new graphic::Text("?", fntData);
    mLabelSize->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelSize);

    // LINE STATUS
    mHeaderStatus = new graphic::Text("STATUS", fntData);
    mHeaderStatus->SetColor(WidgetsConstants::colorPlanetMapHeader);
    RegisterRenderable(mHeaderStatus);

    mLabelStatus = new graphic::Text("?", fntData);
    mLabelStatus->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelStatus);

    // LINE VALUE
    mHeaderValue = new graphic::Text("VALUE", fntData);
    mHeaderValue->SetColor(WidgetsConstants::colorPlanetMapHeader);
    RegisterRenderable(mHeaderValue);

    tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_STARS_DIS);
    mBarValue = new graphic::Image(tex);
    RegisterRenderable(mBarValue);

    // LINE OCCUPIER
    mHeaderOccupier = new graphic::Text("OCCUPIER", fntData);
    mHeaderOccupier->SetColor(WidgetsConstants::colorPlanetMapHeader);
    RegisterRenderable(mHeaderOccupier);

    mLabelOccupier = new graphic::Text("?", fntData);
    mLabelOccupier->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelOccupier);

    // position elements
    UpdatePositions();
}

void PanelPlanetInfo::ClearData()
{
    SetTerritoryValue(0);
    SetTerritorySize(0);
    SetTerritoryStatus(TER_ST_UNKNOWN);
    SetTerritoryOccupier(NO_FACTION);
}

void PanelPlanetInfo::SetTerritorySize(int size)
{
    using namespace sgl;

    if(size == mSize)
        return ;

    mSize = size;

    // delete current text
    UnregisterRenderable(mLabelSize);
    delete mLabelSize;

    // create new text
    auto fm = graphic::FontManager::Instance();

    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(size > 0)
    {
        std::ostringstream s;
        s << size << "x" << size;

        mLabelSize = new graphic::Text(s.str().c_str(), fntData);
    }
    else
        mLabelSize = new graphic::Text("?", fntData);

    mLabelSize->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelSize);

    UpdatePositions();
}

void PanelPlanetInfo::SetTerritoryStatus(TerritoryStatus status)
{
    using namespace sgl;

    if(status == mStatus)
        return ;

    mStatus = status;

    // delete current text
    UnregisterRenderable(mLabelStatus);
    delete mLabelStatus;

    // create new text
    auto fm = graphic::FontManager::Instance();

    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(status < NUM_TERRITORY_STATUSES)
    {
        const char * statuses[] =
        {
            "UNEXPLORED",
            "FREE",
            "OCCUPIED",
            "UNREACHABLE",
            "UNAVAILABLE"
        };

        mLabelStatus = new graphic::Text(statuses[status], fntData);
    }
    else
        mLabelStatus = new graphic::Text("?", fntData);

    mLabelStatus->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelStatus);

    UpdatePositions();
}

void PanelPlanetInfo::SetTerritoryOccupier(PlayerFaction faction)
{
    using namespace sgl;

    if(faction == mOccupier)
        return ;

    mOccupier = faction;

    // delete current text
    UnregisterRenderable(mLabelOccupier);
    delete mLabelOccupier;

    // create new text
    auto fm = graphic::FontManager::Instance();

    const char * fileFont = "data/fonts/Lato-Regular.ttf";
    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(faction < NUM_FACTIONS)
    {
        mLabelOccupier = new graphic::Text(FACTIONS_NAME[faction], fntData);
        mLabelOccupier->SetColor(PLAYER_COLOR[faction]);
    }
    else
    {
        mLabelOccupier = new graphic::Text("?", fntData);
        mLabelOccupier->SetColor(WidgetsConstants::colorPlanetMapData);
    }

    RegisterRenderable(mLabelOccupier);

    UpdatePositions();
}

void PanelPlanetInfo::SetTerritoryValue(unsigned int value)
{
    using namespace sgl;

    const int maxVal = 5;

    if(value > maxVal)
        return ;

    auto tm = graphic::TextureManager::Instance();
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_STARS_DIS + value);
    mBarValue->SetTexture(tex);
}

void PanelPlanetInfo::HandlePositionChanged()
{
    UpdatePositions();
}

void PanelPlanetInfo::HandleStateEnabled()
{

}

void PanelPlanetInfo::HandleStateDisabled()
{

}

void PanelPlanetInfo::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    const int marginL = 20;
    const int marginR = 30;
    const int marginT = 15;

    const int x1 = x0 + GetWidth() - marginR;

    int x;
    int y;
    int dataX;

    // BACKGROUND
    mBg->SetPosition(x0, y0);

    // TITLE
    x = x0 + marginL;
    y = y0 + marginT;

    mTitle->SetPosition(x, y);

    const int marginTitleB = 30;
    const int marginHeaderB = 20;

    // LINE SIZE
    y += mTitle->GetHeight() + marginTitleB;
    mHeaderSize->SetPosition(x, y);

    dataX = x1 - mLabelSize->GetWidth();
    mLabelSize->SetPosition(dataX, y);

    // LINE STATUS
    y += mHeaderSize->GetHeight() + marginHeaderB;
    mHeaderStatus->SetPosition(x, y);

    dataX = x1 - mLabelStatus->GetWidth();
    mLabelStatus->SetPosition(dataX, y);

    // LINE VALUE
    y += mHeaderStatus->GetHeight() + marginHeaderB;
    mHeaderValue->SetPosition(x, y);

    dataX = x1 - mBarValue->GetWidth();
    const int marginValueT = -2;
    const int barY = y + marginValueT;
    mBarValue->SetPosition(dataX, barY);

    // LINE OCCUPIER
    y += mHeaderValue->GetHeight() + marginHeaderB;
    mHeaderOccupier->SetPosition(x, y);

    dataX = x1 - mLabelOccupier->GetWidth();
    mLabelOccupier->SetPosition(dataX, y);
}

} // namespace game
