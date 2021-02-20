#pragma once

#include "IsoObject.h"

namespace lib { namespace graphic { class Text; } }

namespace game
{

class MoveIndicator : public IsoObject
{
public:
    MoveIndicator();

    void SetCost(float val);
    void SetCostUnknown();

    void Render() override;

private:
    static float const COST_UNKNOWN;

private:
    lib::graphic::Text * mTxtCost = nullptr;
    float mCost = COST_UNKNOWN;
};

inline void MoveIndicator::SetCostUnknown() { SetCost(COST_UNKNOWN); }

} // namespace game
