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

enum PlayerFaction : unsigned int;
enum UnitType : unsigned int;

class DialogNewUnit : public lib::sgui::Widget
{
public:
    DialogNewUnit(PlayerFaction faction);
    ~DialogNewUnit();

    void AddUnitSlot(UnitType type, int costEnergy, int costMaterial);

    void SetFunctionOnClose(const std::function<void()> & f);

private:
    void RepositionElements();

    void CreateHeadersPanel();

    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:
    std::vector<SlotDialogNewUnit *> mSlots;

    lib::sgui::Image * mBgLeft = nullptr;
    lib::sgui::Image * mBgRight = nullptr;

    ButtonCloseDialog * mButtonClose = nullptr;

    PlayerFaction mFaction;
};

} // namespace game
