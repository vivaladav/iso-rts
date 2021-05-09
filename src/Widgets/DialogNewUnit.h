#pragma once

#include <sgui/Widget.h>

#include <functional>
#include <vector>

namespace lib
{
    namespace sgui { class Image; }
}

namespace game
{

class ButtonCloseDialog;
class Player;
class SlotDialogNewUnit;
struct UnitData;

enum UnitType : unsigned int;

class DialogNewUnit : public lib::sgui::Widget
{
public:
    DialogNewUnit(Player * player);
    ~DialogNewUnit();

    void AddUnitSlot(const game::UnitData & data);

    UnitType GetTypeToBuild() const;

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

    void UpdateSlots();

private:
    void RepositionElements();

    void CreateHeadersPanel();

    void UpdateSlotButton(const game::UnitData & data, SlotDialogNewUnit * slot);

    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:
    std::vector<SlotDialogNewUnit *> mSlots;

    std::function<void()> mOnBuild;

    Player * mPlayer = nullptr;

    lib::sgui::Image * mBgLeft = nullptr;
    lib::sgui::Image * mBgRight = nullptr;

    ButtonCloseDialog * mButtonClose = nullptr;

    UnitType mTypeToBuild;
};

inline UnitType DialogNewUnit::GetTypeToBuild() const { return mTypeToBuild; }

} // namespace game
