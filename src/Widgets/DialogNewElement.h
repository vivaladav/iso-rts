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
class ButtonLeft;
class ButtonRight;
class ButtonSlot;
class PanelAttribute;
class Player;

struct ObjectData;

enum UnitType : unsigned int;

class DialogNewElement : public lib::sgui::Widget
{
public:
    DialogNewElement(const std::vector<ObjectData> & data, const char * title, Player * player);

    void CheckBuild();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

    int GetSelectedIndex() const;
    const ObjectData & GetSelectedData() const;

private:
    void UpdateSlots();

    void ShowData(int ind);

    void CheckBuild(int ind);

private:
    static const int PANELS_ATT_ROWS = 6;
    static const int PANELS_ATT_COLS = 2;
    static const int NUM_PANELS_ATT = PANELS_ATT_ROWS * PANELS_ATT_COLS;

    static const int NUM_COSTS = 4;

private:
    std::array<PanelAttribute *, NUM_PANELS_ATT> mPanelsAtt;
    std::array<lib::sgui::Label *, NUM_COSTS> mLabelsCost;

    const std::vector<ObjectData> & mData;

    lib::sgui::Label * mTitle = nullptr;
    lib::sgui::ButtonsGroup * mSlots = nullptr;

    lib::sgui::Label * mLabelCategory = nullptr;

    ButtonBuild * mBtnBuild = nullptr;
    ButtonClose * mBtnClose = nullptr;
    ButtonLeft * mBtnLeft = nullptr;
    ButtonRight * mBtnRight = nullptr;

    Player * mPlayer = nullptr;

    int mFirstElem = 0;
};

} // namespace game
