#pragma once

#include <sgui/Widget.h>

#include <vector>

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

    // value has to be in the range [0-10]
    void SetStatValue(unsigned int index, unsigned int val);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::vector<lib::graphic::Image *> mBarsPoints;

    lib::graphic::Image * mBg = nullptr;

    lib::graphic::Image * mPanelUnit = nullptr;
    lib::graphic::Image * mPanelStats = nullptr;
};

} // namespace game
