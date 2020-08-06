#pragma once

#include <sgui/ProgressBar.h>

namespace lib { namespace graphic { class Image; } }

namespace game
{

class CellProgressBar : public lib::sgui::ProgressBar
{
public:
    CellProgressBar(int player, lib::sgui::Widget * parent = nullptr);
    CellProgressBar(int player, float min, float max, lib::sgui::Widget * parent = nullptr);
    ~CellProgressBar();

private:
    void HandlePositionChanged() override;

    void HandleProgressUpdate() override;

    void OnRender() override;

private:
    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Image * mBar = nullptr;

    int mBarW;
    int mBarH;
};

} // namespace game
