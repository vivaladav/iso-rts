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

const char * fileFont = "Lato-Regular.ttf";

PanelPlanetInfo::PanelPlanetInfo()
    : sgl::sgui::Widget(nullptr)
    , mOccupier(NO_FACTION)
    , mStatus(TER_ST_UNKNOWN)
    , mMission(MISSION_UNKNOWN)
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

    // LINE MISSION
    mHeaderMission = new graphic::Text("MISSION", fntData);
    mHeaderMission->SetColor(WidgetsConstants::colorPlanetMapHeader);
    RegisterRenderable(mHeaderMission);

    mLabelMission = new graphic::Text("?", fntData);
    mLabelMission->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelMission);

    // position elements
    UpdatePositions();
}

void PanelPlanetInfo::ClearData()
{
    if(TER_ST_UNKNOWN == mStatus)
        return ;

    mValue = 0;
    mSize = 0;
    mStatus = TER_ST_UNKNOWN;
    mOccupier = NO_FACTION;

    UpdateTerritorySize();
    UpdateTerritoryStatus();
    UpdateTerritoryOccupier();
    UpdateTerritoryValue();

    UpdatePositions();
}

void PanelPlanetInfo::SetData(int size, TerritoryStatus status, PlayerFaction faction,
                              unsigned int value, MissionType mission)
{
    const bool sizeChanged = size != mSize;
    const bool statusChanged = status != mStatus;
    const bool factionChanged = faction != mOccupier;
    const bool valueChanged = value != mValue;
    const bool missionChanged = mission != mMission;

    mValue = value;
    mSize = size;
    mStatus = status;
    mOccupier = faction;
    mMission = mission;

    if(sizeChanged)
        UpdateTerritorySize();

    if(statusChanged)
        UpdateTerritoryStatus();

    if(factionChanged || (NO_FACTION == mOccupier && statusChanged))
        UpdateTerritoryOccupier();

    if(valueChanged)
        UpdateTerritoryValue();

    if(missionChanged)
        UpdateMissionType();

    UpdatePositions();
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
    const int marginHeaderB = 10;

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

    // LINE MISSION
    y += mHeaderSize->GetHeight() + marginHeaderB;
    mHeaderMission->SetPosition(x, y);

    dataX = x1 - mLabelMission->GetWidth();
    mLabelMission->SetPosition(dataX, y);
}

void PanelPlanetInfo::UpdateTerritorySize()
{
    using namespace sgl;

    // delete current text
    UnregisterRenderable(mLabelSize);
    delete mLabelSize;

    // create new text
    auto fm = graphic::FontManager::Instance();

    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(mSize > 0)
    {
        std::ostringstream s;
        s << mSize << "x" << mSize;

        mLabelSize = new graphic::Text(s.str().c_str(), fntData);
    }
    else
        mLabelSize = new graphic::Text("?", fntData);

    mLabelSize->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelSize);
}

void PanelPlanetInfo::UpdateTerritoryStatus()
{
    using namespace sgl;

    // delete current text
    UnregisterRenderable(mLabelStatus);
    delete mLabelStatus;

    // create new text
    auto fm = graphic::FontManager::Instance();

    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(mStatus < NUM_TERRITORY_STATUSES)
    {
        const char * statuses[] =
        {
            "UNEXPLORED",
            "FREE",
            "OCCUPIED",
            "OCCUPIED",
            "UNREACHABLE",
            "UNAVAILABLE"
        };

        mLabelStatus = new graphic::Text(statuses[mStatus], fntData);
    }
    else
        mLabelStatus = new graphic::Text("?", fntData);

    mLabelStatus->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelStatus);
}

void PanelPlanetInfo::UpdateTerritoryOccupier()
{
    using namespace sgl;

    // delete current text
    UnregisterRenderable(mLabelOccupier);
    delete mLabelOccupier;

    // create new text
    auto fm = graphic::FontManager::Instance();

    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                          graphic::Font::NORMAL);

    if(mOccupier < NUM_FACTIONS)
    {
        mLabelOccupier = new graphic::Text(FACTIONS_NAME[mOccupier], fntData);
        mLabelOccupier->SetColor(PLAYER_COLOR[mOccupier]);
    }
    else
    {
        if(mStatus != TER_ST_UNKNOWN && mStatus != TER_ST_UNEXPLORED)
            mLabelOccupier = new graphic::Text("-", fntData);
        else
            mLabelOccupier = new graphic::Text("?", fntData);

        mLabelOccupier->SetColor(WidgetsConstants::colorPlanetMapData);
    }

    RegisterRenderable(mLabelOccupier);
}

void PanelPlanetInfo::UpdateTerritoryValue()
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();
    graphic::Texture * tex = tm->GetSprite(SpriteFilePlanetMap, IND_PM_STARS_DIS + mValue);
    mBarValue->SetTexture(tex);
}

void PanelPlanetInfo::UpdateMissionType()
{
    using namespace sgl;

    // delete current text
    UnregisterRenderable(mLabelMission);
    delete mLabelMission;

    // create new text
    auto fm = graphic::FontManager::Instance();

    graphic::Font * fntData = fm->GetFont(fileFont, WidgetsConstants::FontSizePlanetMapText,
                                         graphic::Font::NORMAL);

    if(mMission < NUM_MISSION_TYPES)
        mLabelMission = new graphic::Text(MISSIONS_TITLE[mMission], fntData);
    else if(MISSION_COMPLETED == mMission)
        mLabelMission = new graphic::Text("-", fntData);
    // UNKNOWN
    else
        mLabelMission = new graphic::Text("?", fntData);

    mLabelMission->SetColor(WidgetsConstants::colorPlanetMapData);
    RegisterRenderable(mLabelMission);
}

} // namespace game
