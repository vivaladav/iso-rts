#pragma once

#include <sgui/Widget.h>

#include <array>
#include <functional>
#include <vector>

namespace lib
{
    namespace sgui { class ButtonsGroup; }
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
    DialogNewElement(Player * player);
    ~DialogNewElement();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

private:
    static const int PANELS_ATT_ROWS = 6;
    static const int PANELS_ATT_COLS = 2;
    static const int NUM_PANELS_ATT = PANELS_ATT_ROWS * PANELS_ATT_COLS;

private:
    std::array<PanelAttribute *, NUM_PANELS_ATT> mPanelsAtt;

    lib::sgui::ButtonsGroup * mSlots = nullptr;

    ButtonBuild * mBtnBuild = nullptr;
    ButtonClose * mBtnClose = nullptr;
};

} // namespace game
