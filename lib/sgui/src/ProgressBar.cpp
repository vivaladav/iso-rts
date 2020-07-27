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
    {
        Complete();
        return ;
    }
    else
        mValue = progress;

    if((mMax - mValue) < delta)
        Complete();
    else
        HandleProgressUpdate();
}

void ProgressBar::IncValue(float val)
{
    mValue += val;

    if((mMax - mValue) < delta)
        Complete();
    else
        HandleProgressUpdate();
}

void ProgressBar::SetValuePerc(float perc)
{
    if(mCompleted)
        return ;

    if(perc < 0.f)
        perc = 0.f;
    else if(perc > 100.f)
    {
        Complete();
        return ;
    }

    mValue = perc * (mMax - mMin) / 100.f;

    if((100.f - perc) < delta)
        Complete();
    else
        HandleProgressUpdate();
}

void ProgressBar::Reset()
{
    mCompleted = false;
    mValue = mMin;

    HandleProgressUpdate();
}

void ProgressBar::Complete()
{
    mCompleted = true;
    mValue = mMax;

    HandleProgressUpdate();

    mFunOnCompleted();
}

} // namespace sgui
} // namespace lib
