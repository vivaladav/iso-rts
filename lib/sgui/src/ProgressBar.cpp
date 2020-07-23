#include "sgui/ProgressBar.h"

namespace lib
{
namespace sgui
{

void ProgressBar::SetValue(float progress)
{
    if(progress < mMin)
        mValue = mMin;
    else if(progress > mMax)
        mValue = mMax;
    else
        mValue = progress;

    HandleProgressUpdate();
}

void ProgressBar::SetValuePerc(float perc)
{
    mValue = perc * (mMax - mMin) / 100.f;

    HandleProgressUpdate();
}

} // namespace sgui
} // namespace lib
