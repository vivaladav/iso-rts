#pragma once

#include <sgui/ProgressBar.h>

namespace lib { namespace graphic { class Image; } }

namespace game
{

enum PlayerFaction : unsigned int;

class CellProgressBar : public lib::sgui::ProgressBar
{
public:
    CellProgressBar(PlayerFaction faction, lib::sgui::Widget * parent = nullptr);
    CellProgressBar(PlayerFaction faction, float min, float max, lib::sgui::Widget * parent = nullptr);

private:
    void HandlePositionChanged() override;

    void HandleProgressUpdate() override;

private:
    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Image * mBar = nullptr;

    int mBarW;
    int mBarH;
};

} // namespace game
