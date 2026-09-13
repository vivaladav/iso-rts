#pragma once

#include "Screen.h"

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class ResourceLoader;

class ScreenInitGameSetup : public Screen
{
public:
    ScreenInitGameSetup(Game * game);
    ~ScreenInitGameSetup();

    void Update(float delta) override;
    void Render() override;

private:
    void UpdateStatus();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::sgui::Label * mLabelStatus = nullptr;

    ResourceLoader * mResLoader = nullptr;

    float mTimeLoad = 0.f;
};

} // namespace game
