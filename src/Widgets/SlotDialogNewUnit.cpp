#include "Widgets/SlotDialogNewUnit.h"

#include "GameData.h"
#include "Widgets/ButtonBuildNewUnit.h"
#include "Widgets/GameUIData.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>

namespace game
{

constexpr int NUM_STATS = 6;
constexpr int NUM_BAR_POINTS = 10;
constexpr int NUM_TOT_POINTS = NUM_STATS * NUM_BAR_POINTS;

SlotDialogNewUnit::SlotDialogNewUnit(PlayerFaction faction, UnitType type,
                                     int index, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mBg(new lib::graphic::Image)
    , mType(type)
{
    using namespace lib::graphic;

    // BACKGROUND
    auto tm = TextureManager::Instance();
    Texture * tex = nullptr;

    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_SLOT);
    mBg->SetTexture(tex);
    RegisterRenderable(mBg);

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();

    SetSize(w, h);

    // UNIT PANEL
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_PANEL_UNIT);
    mPanelUnit = new Image(tex);
    RegisterRenderable(mPanelUnit);

    // unit icon
    const unsigned int texUnit = (NUM_UNIT_SPRITES_PER_FACTION * faction) +
                                 (NUM_UNIT_SPRITES_PER_TYPE * mType);
    tex = tm->GetSprite(SpriteFileUnits, texUnit);
    mUnit = new Image(tex);
    RegisterRenderable(mUnit);

    // STATS PANEL
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_PANEL_STATS);
    mPanelStats = new Image(tex);
    RegisterRenderable(mPanelStats);

    // STATS BAR POINTS
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_BAR_ITEM);

    for(int i = 0; i < NUM_TOT_POINTS; ++i)
    {
        auto img = new Image(tex);
        RegisterRenderable(img);

        mBarsPoints.push_back(img);
    }

    // BUTTON BUILD
    const int shortcutKey = lib::core::KeyboardEvent::KEY_1 + index;
    std::string shortcutStr = std::to_string(index + 1);
    mButtonBuild = new ButtonBuildNewUnit(shortcutStr.c_str(), shortcutKey, this);
}

SlotDialogNewUnit::~SlotDialogNewUnit()
{
    delete mBg;

    delete mPanelUnit;
    delete mPanelStats;

    for(auto img : mBarsPoints)
        delete img;
}

void SlotDialogNewUnit::SetStatValue(unsigned int index, unsigned int val)
{
    // index out of bounds
    if(index > NUM_STATS)
        return ;

    if(val > NUM_BAR_POINTS)
        val = NUM_BAR_POINTS;

    const unsigned int colors[] =
    {
        0xf5a3a3ff,
        0xf5daa3ff,
        0xa6f2a6ff,
        0xa3daf5ff
    };

    const unsigned int colorOffsets[] = { 3, 6, 9 };

    const unsigned int colorOff = 0x768089ff;
    int colorOn = colorOff;

    if(val < colorOffsets[0])
        colorOn = colors[0];
    else if(val < colorOffsets[1])
        colorOn = colors[1];
    else if(val < colorOffsets[2])
        colorOn = colors[2];
    else
        colorOn = colors[3];

    const unsigned int index0 = index * NUM_BAR_POINTS;

    // ON points
    for(unsigned int i = 0; i < val; ++i)
    {
        const unsigned int point = index0 + i;
        mBarsPoints[point]->SetColor(colorOn);
    }

    // OFF points
    for(unsigned int i = val; i < NUM_BAR_POINTS; ++i)
    {
        const unsigned int point = index0 + i;
        mBarsPoints[point]->SetColor(colorOff);
    }
}

void SlotDialogNewUnit::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    // background
    mBg->SetPosition(x0, y0);

    // panel unit
    int x = x0;
    int y = y0 + 20;
    mPanelUnit->SetPosition(x, y);

    // unit icon
    const int unitX = x + (mPanelUnit->GetWidth() - mUnit->GetWidth()) * 0.5f;
    y += 23;
    mUnit->SetPosition(unitX, y);

    // panel stats
    y = mPanelUnit->GetY() + mPanelUnit->GetHeight() + 10;
    mPanelStats->SetPosition(x, y);

    // stats bars
    const int barX0 = x + 17;
    y += 14;

    for(int s = 0; s < NUM_STATS; ++s)
    {
        x = barX0;

        const int ind0 = s * NUM_BAR_POINTS;

        for(int p = 0; p < NUM_BAR_POINTS; ++p)
        {
            const int ind = ind0 + p;

            mBarsPoints[ind]->SetPosition(x, y);

            x += mBarsPoints[ind]->GetWidth() * 2;
        }

        y += mBarsPoints[0]->GetHeight() + 15;
    }

    // button BUILD
    const int buttonY = mPanelStats->GetY() + mPanelStats->GetHeight() + 10 - y0;
    mButtonBuild->SetY(buttonY);
}

void SlotDialogNewUnit::OnRender()
{
    mBg->Render();

    mPanelUnit->Render();
    mUnit->Render();

    mPanelStats->Render();

    for(auto img : mBarsPoints)
        img->Render();
}

} // namespace game
