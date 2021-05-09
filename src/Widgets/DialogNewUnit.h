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
class SlotDialogNewUnit;
struct UnitData;

enum PlayerFaction : unsigned int;
enum UnitType : unsigned int;

class DialogNewUnit : public lib::sgui::Widget
{
public:
    DialogNewUnit(PlayerFaction faction);
    ~DialogNewUnit();

    void AddUnitSlot(const game::UnitData & data);

    UnitType GetTypeToBuild() const;

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

private:
    void RepositionElements();

    void CreateHeadersPanel();

    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:
    std::vector<SlotDialogNewUnit *> mSlots;

    std::function<void()> mOnBuild;

    lib::sgui::Image * mBgLeft = nullptr;
    lib::sgui::Image * mBgRight = nullptr;

    ButtonCloseDialog * mButtonClose = nullptr;

    PlayerFaction mFaction;

    UnitType mTypeToBuild;
};

inline UnitType DialogNewUnit::GetTypeToBuild() const { return mTypeToBuild; }

} // namespace game
