#include "sgui/ProgressBar.h"

namespace lib
{
namespace sgui
{

const float delta = 0.001f;

ProgressBar::ProgressBar(float min, float max, Widget * parent)
    : Widget(parent)
    , mFunOnCompleted([]{})
    , mMin(min)
    , mMax(max)
{
}

void ProgressBar::SetValue(float progress)
{
    if(mCompleted)
        return ;

    if(progress < mMin)
        mValue = mMin;
    else if(progress > mMax)
        mValue = mMax;
    else
        mValue = progress;

    HandleProgressUpdate();

    if((mMax - mValue) < delta)
    {
        mCompleted = true;
        mFunOnCompleted();
    }
}

void ProgressBar::SetValuePerc(float perc)
{
    if(mCompleted)
        return ;

    if(perc < 0.f)
        perc = 0.f;
    else if(perc > 100.f)
        perc = 100.f;

    mValue = perc * (mMax - mMin) / 100.f;

    HandleProgressUpdate();

    if((100.f - perc) < delta)
    {
        mCompleted = true;
        mFunOnCompleted();
    }
}

} // namespace sgui
} // namespace lib
