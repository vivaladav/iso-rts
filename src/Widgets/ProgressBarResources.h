#pragma once

#include <sgui/ProgressBar.h>

namespace lib { namespace graphic { class Image; } }

namespace game
{

enum PlayerFaction : unsigned int;

class ProgressBarResources : public lib::sgui::ProgressBar
{
public:
    ProgressBarResources(float min, float max, lib::sgui::Widget * parent = nullptr);
    ~ProgressBarResources();

private:
    void HandlePositionChanged() override;

    void HandleProgressUpdate() override;

    void OnRender() override;

    void SetBarColor();

private:
    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Image * mBar = nullptr;

    int mBarW;
    int mBarH;
};

} // namespace game
