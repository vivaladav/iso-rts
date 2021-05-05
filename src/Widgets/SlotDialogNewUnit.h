#pragma once

#include <sgui/Widget.h>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class SlotDialogNewUnit : public lib::sgui::Widget
{
public:
    SlotDialogNewUnit(lib::sgui::Widget * parent);
    ~SlotDialogNewUnit();

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
