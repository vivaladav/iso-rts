#pragma once

#include "IsoObject.h"

namespace sgl { namespace graphic { class Text; } }

namespace game
{

class MoveIndicator : public IsoObject
{
public:
    enum IndicatorType : unsigned int
    {
        NORMAL,
        NO_VIS_CELL,

        NUM_IND_TYPES,

        UNDEFINED
    };

public:
    MoveIndicator();

    void SetIndicatorType(IndicatorType type);
    void SetCost(float val);
    void SetCostUnknown();

    void Render() override;

private:
    static float const COST_UNKNOWN;

private:
    sgl::graphic::Text * mTxtCost = nullptr;
    unsigned int mColorCost = 0;

    float mCost = COST_UNKNOWN;

    IndicatorType mType = UNDEFINED;
};

inline void MoveIndicator::SetCostUnknown() { SetCost(COST_UNKNOWN); }

} // namespace game
