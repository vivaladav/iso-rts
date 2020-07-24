#pragma once

#include "sgui/Widget.h"

namespace lib
{

namespace sgui
{

class ProgressBar : public Widget
{
public:
    ProgressBar(float min, float max, Widget * parent = nullptr);

    float GetValue() const;
    void SetValue(float progress);
    float GetValuePerc() const;
    void SetValuePerc(float perc);

protected:
    virtual void HandleProgressUpdate() = 0;

private:
    float mValue = 0;
    float mMin = 0;
    float mMax = 100;
};


inline ProgressBar::ProgressBar(float min, float max, Widget * parent)
    : Widget(parent)
    , mMin(min)
    , mMax(max)
{
}

inline float ProgressBar::GetValue() const { return mValue; }

inline float ProgressBar::GetValuePerc() const
{
    return mValue * 100.f / (mMax - mMin);
}


} // namespace sgui
} // namespace lib
