#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }

    namespace sgui { class Label; }
}

namespace game
{

enum PlayerFaction : unsigned int;
enum TerritoryStatus : unsigned int;

class PanelPlanetInfo : public sgl::sgui::Widget
{
public:
    PanelPlanetInfo();

    void ClearData();

    void SetTerritorySize(int size);
    void SetTerritoryStatus(TerritoryStatus status);
    void SetTerritoryOccupier(PlayerFaction faction);
    void SetTerritoryValue(unsigned int value);

private:
    void HandlePositionChanged() override;

    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void UpdatePositions();

private:
    sgl::graphic::Text * mTitle = nullptr;
    sgl::graphic::Text * mHeaderSize = nullptr;
    sgl::graphic::Text * mHeaderStatus = nullptr;
    sgl::graphic::Text * mHeaderValue = nullptr;
    sgl::graphic::Text * mHeaderOccupier = nullptr;

    sgl::graphic::Text * mLabelSize = nullptr;
    sgl::graphic::Text * mLabelStatus = nullptr;
    sgl::graphic::Text * mLabelOccupier = nullptr;
    sgl::graphic::Image * mBarValue = nullptr;

    sgl::graphic::Image * mBg = nullptr;

    int mSize = 0;
    int mValue = 0;
    PlayerFaction mOccupier;
    TerritoryStatus mStatus;

};

} // namespace game
