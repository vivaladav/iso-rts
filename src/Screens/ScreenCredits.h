#pragma once

#include "Screen.h"

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class AbstractButtonsGroup; }
}

namespace game
{

class DialogCredits;

class ScreenCredits : public Screen
{
public:
    ScreenCredits(Game * game);
    ~ScreenCredits();

    void Update(float update) override;
    void Render() override;

private:
    sgl::graphic::Image * mBg = nullptr;

    DialogCredits * mDialog = nullptr;
};

} // namespace game
