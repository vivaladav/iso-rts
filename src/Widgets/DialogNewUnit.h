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

class DialogNewUnit : public lib::sgui::Widget
{
public:
    DialogNewUnit();
    ~DialogNewUnit();

    void AddUnitSlot();

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
};

} // namespace game
