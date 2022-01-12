#pragma once

#include <sgui/Widget.h>

#include <array>
#include <functional>
#include <vector>

namespace lib
{
    namespace sgui
    {
        class ButtonsGroup;
        class Label;
    }
}

namespace game
{

class ButtonBuild;
class ButtonClose;
class ButtonSlot;
class PanelAttribute;
class Player;

enum UnitType : unsigned int;

class DialogNewElement : public lib::sgui::Widget
{
public:
    DialogNewElement(Player * player, const char * title);

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

private:
    static const int PANELS_ATT_ROWS = 6;
    static const int PANELS_ATT_COLS = 2;
    static const int NUM_PANELS_ATT = PANELS_ATT_ROWS * PANELS_ATT_COLS;

    static const int NUM_COSTS = 4;

private:
    std::array<PanelAttribute *, NUM_PANELS_ATT> mPanelsAtt;
    std::array<lib::sgui::Label *, NUM_COSTS> mLabelsCost;

    lib::sgui::Label * mTitle = nullptr;
    lib::sgui::ButtonsGroup * mSlots = nullptr;

    lib::sgui::Label * mLabelCategory = nullptr;

    ButtonBuild * mBtnBuild = nullptr;
    ButtonClose * mBtnClose = nullptr;
};

} // namespace game
