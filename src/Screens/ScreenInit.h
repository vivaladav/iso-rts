#pragma once

#include "Screen.h"

#include <functional>
#include <vector>

namespace lib
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
    lib::graphic::Image * mBg = nullptr;
    lib::sgui::Label * mLabelStatus = nullptr;

    std::vector<std::function<void()>> mJobs;
    unsigned int mCurrJob = 0;
};

} // namespace game
