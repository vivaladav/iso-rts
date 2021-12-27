#include "Widgets/SlotDialogNewUnit.h"

/*
#include "GameData.h"
#include "GameObjects/UnitData.h"
#include "Widgets/ButtonBuildNewUnit.h"
#include "Widgets/GameUIData.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Camera.h>
#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Text.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>

namespace game
{

constexpr int NUM_STATS = 6;
constexpr int NUM_BAR_POINTS = 10;
constexpr int NUM_TOT_POINTS = NUM_STATS * NUM_BAR_POINTS;

SlotDialogNewUnit::SlotDialogNewUnit(PlayerFaction faction, const UnitData & data, int index, lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mBg(new lib::graphic::Image)
    , mType(data.type)
{
    using namespace lib::graphic;

    auto fm = FontManager::Instance();
    Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 16, lib::graphic::Font::NORMAL);

    auto tm = TextureManager::Instance();
    Texture * tex = nullptr;

    // BACKGROUND
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

    const unsigned int colorCosts = 0xe3e6e8ff;

    // unit icon
    const unsigned int texUnit = (NUM_UNIT_SPRITES_PER_FACTION * faction) +
                                 (NUM_UNIT_SPRITES_PER_TYPE * mType);
    tex = tm->GetSprite(SpriteFileUnits, texUnit);
    mIconUnit = new Image(tex);
    RegisterRenderable(mIconUnit);

    // icon energy
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_ICON_ENERGY);
    mIconEnergy = new Image(tex);
    RegisterRenderable(mIconEnergy);

    // text energy
    mTextEnergy = new Text(std::to_string(data.costEnergy).c_str(), font, true);
    mTextEnergy->SetColor(colorCosts);
    RegisterRenderable(mTextEnergy);

    // icon material
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_ICON_MATERIAL);
    mIconMaterial = new Image(tex);
    RegisterRenderable(mIconMaterial);

    // text material
    mTextMaterial = new Text(std::to_string(data.costMaterial).c_str(), font, true);
    mTextMaterial->SetColor(colorCosts);
    RegisterRenderable(mTextMaterial);

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

    // set stats
    SetStatValue(USTAT_ENERGY, data.statEnergy);
    SetStatValue(USTAT_RESISTANCE, data.statResistance);
    SetStatValue(USTAT_ATTACK, data.statAttack);
    SetStatValue(USTAT_SPEED, data.statSpeed);
    SetStatValue(USTAT_CONSTRUCTION, data.statConstruction);
    SetStatValue(USTAT_CONQUEST, data.statConquest);

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

    delete mIconUnit;
    delete mIconEnergy;
    delete mIconMaterial;

    for(auto img : mBarsPoints)
        delete img;
}

void SlotDialogNewUnit::SetButtonEnabled(bool enabled)
{
    mButtonBuild->SetEnabled(enabled);
}

void SlotDialogNewUnit::SetOnBuildFunction(const std::function<void()> & f)
{
    mButtonBuild->AddOnClickFunction(f);
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
    const int unitX = x + (mPanelUnit->GetWidth() - mIconUnit->GetWidth()) * 0.5f;
    y += 20;
    mIconUnit->SetPosition(unitX, y);

    // energy and material costs
    const int costW = 94;
    const int costX0 = x + (mPanelUnit->GetWidth() - costW) * 0.5f;
    const int costY0 = mPanelUnit->GetY() + mPanelUnit->GetHeight() - mIconEnergy->GetHeight() - 15;
    const int costIconMargin = 5;

    mIconEnergy->SetPosition(costX0, costY0);

    const int txtEnergyX = mIconEnergy->GetX() + mIconEnergy->GetWidth() + costIconMargin;
    const int txtEnergyY = mIconEnergy->GetY() + (mIconEnergy->GetHeight() - mTextEnergy->GetHeight()) * 0.5f;
    mTextEnergy->SetPosition(txtEnergyX, txtEnergyY);

    const int txtMaterialX = costX0 + costW - mTextMaterial->GetWidth();
    const int txtMaterialY = txtEnergyY;
    mTextMaterial->SetPosition(txtMaterialX, txtMaterialY);

    const int materialX = txtMaterialX - costIconMargin - mIconMaterial->GetWidth();
    const int materialY = mIconEnergy->GetY() + (mIconEnergy->GetHeight() - mIconMaterial->GetHeight()) * 0.5f;
    mIconMaterial->SetPosition(materialX, materialY);

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
    mIconUnit->Render();
    mIconEnergy->Render();
    mTextEnergy->Render();
    mIconMaterial->Render();
    mTextMaterial->Render();

    mPanelStats->Render();

    for(auto img : mBarsPoints)
        img->Render();
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

} // namespace game
*/
