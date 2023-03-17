#pragma once

#include <sgl/sgui/Widget.h>

#include <array>
#include <functional>
#include <vector>

namespace sgl
{
    namespace graphic { class Image; }

    namespace sgui
    {
        class AbstractButtonsGroup;
        class ButtonsGroup;
        class Label;
        class TextArea;
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

enum ObjCategory : unsigned int;
enum UnitType : unsigned int;

class DialogNewElement : public sgl::sgui::Widget
{
public:
    enum ElemType : unsigned int
    {
        ETYPE_UNITS,
        ETYPE_STRUCTURES,
    };

    DialogNewElement(ElemType type, const char * title, Player * player);

    void CheckBuild();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

    int GetSelectedIndex() const;
    const ObjectData & GetSelectedData() const;

private:
    void UpdateSlots();

    void ShowStructuresByCategory(ObjCategory cat);
    void ShowData(int ind);

    void CheckBuild(int ind);

    void HandlePositionChanged() override;
    void PositionElements();

private:
    static const int PANELS_ATT_ROWS = 7;
    static const int PANELS_ATT_COLS = 2;
    static const int NUM_PANELS_ATT = PANELS_ATT_ROWS * PANELS_ATT_COLS;

    static const int NUM_COSTS = 4;

private:
    std::array<PanelAttribute *, NUM_PANELS_ATT> mPanelsAtt;
    std::array<sgl::sgui::Label *, NUM_COSTS> mLabelsCost;

    std::vector<ObjectData> mData;

    sgl::graphic::Image * mBgTop = nullptr;
    sgl::graphic::Image * mBgMid = nullptr;
    sgl::graphic::Image * mBgBtm = nullptr;

    sgl::sgui::Label * mTitle = nullptr;
    sgl::sgui::ButtonsGroup * mSlots = nullptr;

    sgl::sgui::TextArea * mDescription = nullptr;
    sgl::sgui::Label * mCategory = nullptr;

    ButtonBuild * mBtnBuild = nullptr;
    ButtonClose * mBtnClose = nullptr;
    ButtonLeft * mBtnLeft = nullptr;
    ButtonRight * mBtnRight = nullptr;

    sgl::sgui::AbstractButtonsGroup * mButtonsStructures = nullptr;

    Player * mPlayer = nullptr;

    ElemType mElemType;

    int mFirstElem = 0;
};

} // namespace game
