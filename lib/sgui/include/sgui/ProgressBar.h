#pragma once

#include "sgui/Widget.h"

#include <functional>

namespace lib
{
namespace sgui
{

class ProgressBar : public Widget
{
public:
    ProgressBar(float min, float max, Widget * parent = nullptr);

    void SetMinMax(float min, float max);

    float GetValue() const;
    void SetValue(float progress);
    void IncValue(float val);
    float GetValuePerc() const;
    void SetValuePerc(float perc);

    bool IsCompleted() const;

    void Reset();

    void SetFunctionOnCompleted(const std::function<void()> & f);

protected:
    virtual void HandleProgressUpdate() = 0;

private:
    void Complete();

private:
    std::function<void()> mFunOnCompleted;

    float mValue = 0;
    float mMin = 0;
    float mMax = 100;

    bool mCompleted = false;
};

inline float ProgressBar::GetValue() const { return mValue; }

inline float ProgressBar::GetValuePerc() const
{
    return mValue * 100.f / (mMax - mMin);
}

inline void ProgressBar::SetFunctionOnCompleted(const std::function<void()> & f)
{
    mFunOnCompleted = f;
}

inline bool ProgressBar::IsCompleted() const { return mCompleted; }

} // namespace sgui
} // namespace lib
