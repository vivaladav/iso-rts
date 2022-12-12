#pragma once

#include "Screen.h"

#include <functional>
#include <vector>

namespace sgl
{
    namespace core { class DataPackage; }
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
    enum Packages : unsigned int
    {
        PACKAGE_GAME,
        PACKAGE_TEST,
        PACKAGE_UI,

        NUM_DATA_PACKAGES
    };

    sgl::graphic::Image * mBg = nullptr;
    sgl::sgui::Label * mLabelStatus = nullptr;

    std::vector<sgl::core::DataPackage *> mPackages;

    std::vector<std::function<void()>> mJobs;
    unsigned int mCurrJob = 0;
};

} // namespace game
