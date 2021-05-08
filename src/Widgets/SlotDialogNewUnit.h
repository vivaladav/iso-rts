#pragma once

#include <sgui/Widget.h>

#include <vector>

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class ButtonBuildNewUnit;

enum PlayerFaction : unsigned int;
enum UnitType : unsigned int;

class SlotDialogNewUnit : public lib::sgui::Widget
{
public:
    SlotDialogNewUnit(PlayerFaction faction, UnitType type, int costEnergy,
                      int costMaterial, int index, lib::sgui::Widget * parent);
    ~SlotDialogNewUnit();

    UnitType GetUnitType() const;

    // value has to be in the range [0-10]
    void SetStatValue(unsigned int index, unsigned int val);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::vector<lib::graphic::Image *> mBarsPoints;

    lib::graphic::Image * mBg = nullptr;

    lib::graphic::Image * mPanelUnit = nullptr;
    lib::graphic::Image * mPanelStats = nullptr;

    lib::graphic::Image * mIconUnit = nullptr;
    lib::graphic::Image * mIconEnergy = nullptr;
    lib::graphic::Image * mIconMaterial = nullptr;
    lib::graphic::Text * mTextEnergy = nullptr;
    lib::graphic::Text * mTextMaterial = nullptr;

    ButtonBuildNewUnit * mButtonBuild = nullptr;

    UnitType mType;
};

} // namespace game
