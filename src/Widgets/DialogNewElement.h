#pragma once

#include "GameObjects/GameObjectTypes.h"

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
class ObjectsDataRegistry;
class PanelAttribute;
class Player;

enum ObjFamily : unsigned int;

class DialogNewElement : public sgl::sgui::Widget
{
public:
    enum ElemType : unsigned int
    {
        ETYPE_UNITS,
        ETYPE_STRUCTURES,

        NUM_ELEMENT_TYPES
    };

    DialogNewElement(ElemType type, Player * player, const ObjectsDataRegistry * dataReg);

    void CheckBuild();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

    int GetSelectedIndex() const;
    GameObjectTypeId GetSelectedType() const;

private:
    void UpdateSlots();

    void ShowStructuresByFamily(ObjFamily fam);
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

    std::vector<GameObjectTypeId> mTypes;

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

    const ObjectsDataRegistry * mDataReg;

    ElemType mElemType;

    int mFirstElem = 0;
};

} // namespace game
