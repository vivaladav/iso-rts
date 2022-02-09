#pragma once

#include <sgl/sgui/ProgressBar.h>

namespace sgl { namespace graphic { class Image; } }

namespace game
{

enum PlayerFaction : unsigned int;

class ProgressBarResources : public sgl::sgui::ProgressBar
{
public:
    ProgressBarResources(float min, float max, sgl::sgui::Widget * parent = nullptr);

private:
    void HandlePositionChanged() override;

    void HandleProgressUpdate() override;

    void SetBarColor();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Image * mBar = nullptr;

    int mBarW;
    int mBarH;
};

} // namespace game
