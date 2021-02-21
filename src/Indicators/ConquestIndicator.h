#pragma once

#include "IsoObject.h"

namespace lib { namespace graphic { class Text; } }

namespace game
{

enum PlayerFaction : unsigned int;

class ConquestIndicator : public IsoObject
{
public:
    ConquestIndicator();

    void SetFaction(PlayerFaction faction);

    void SetCost(float val);
    void ShowCost(bool val);

    void Render() override;

private:
    void CreateLabelCost();

private:
    lib::graphic::Text * mTxtCost = nullptr;
    unsigned int mColorCost = 0;

    float mCost = 0.f;
};

} // namespace game
