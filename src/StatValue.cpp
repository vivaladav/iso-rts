#include "StatValue.h"

#include <cmath>
#include <limits>

namespace game
{

StatValue::StatValue(unsigned int statId, int val)
    : mId(statId)
    , mData(val)
    , mMin(std::numeric_limits<int>::min())
    , mMax(std::numeric_limits<int>::max())
{
}

StatValue::StatValue(unsigned int statId, float val)
    : mId(statId)
    , mData(val)
    , mMin(std::numeric_limits<float>::min())
    , mMax(std::numeric_limits<float>::max())
{
}

void StatValue::SetMin(int min)
{
    mMin.d = min;

    if(mData.d < mMin.d)
    {
        mData.d = mMin.d;
        NotifyObserversValue();
    }

    NotifyObserversRange();
}

void StatValue::SetMin(float min)
{
    mMin.f = min;

    if(mData.f < mMin.f)
    {
        mData.f = mMin.f;
        NotifyObserversValue();
    }

    NotifyObserversRange();
}

void StatValue::SetMax(int max)
{
    mMax.d = max;

    if(mData.d > mMax.d)
    {
        mData.d = mMax.d;
        NotifyObserversValue();
    }

    NotifyObserversRange();
}

void StatValue::SetMax(float max)
{
    mMax.f = max;

    if(mData.f > mMax.f)
    {
        mData.f = mMax.f;
        NotifyObserversValue();
    }

    NotifyObserversRange();
}

void StatValue::SetValue(int val)
{
    // same value -> exit
    if(val == mData.d)
        return ;

    mData.d = val;

    NotifyObserversValue();
}

void StatValue::SetValue(float val)
{
    const float delta = 0.0001f;

    // almost same value -> exit
    if(std::fabs(mData.f - val) < delta)
        return ;

    mData.d = val;

    NotifyObserversValue();
}

void StatValue::SumValue(int val)
{
    mData.d += val;

    // clamp data
    if(mData.d < mMin.d)
        mData.d = mMin.d;
    else if(mData.d > mMax.d)
        mData.d = mMax.d;

    NotifyObserversValue();
}

void StatValue::SumValue(float val)
{
    mData.f += val;

    // clamp data
    if(mData.f < mMin.f)
        mData.f = mMin.f;
    else if(mData.f > mMax.f)
        mData.f = mMax.f;

    NotifyObserversValue();
}

void StatValue::NotifyObserversValue()
{
    for(auto & f : mCallbacksVal)
        f(this);
}

void StatValue::NotifyObserversRange()
{
    for(auto & f : mCallbacksRange)
        f(this);
}

} // namespace game
