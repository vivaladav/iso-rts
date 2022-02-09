#pragma once

#include "Screen.h"

#include <functional>
#include <vector>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class ScreenInit : public Screen
{
public:
    ScreenInit(Game * game);
    ~ScreenInit();

    void Update(float update) override;
    void Render() override;

private:
    void UpdateStatus();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::sgui::Label * mLabelStatus = nullptr;

    std::vector<std::function<void()>> mJobs;
    unsigned int mCurrJob = 0;
};

} // namespace game
