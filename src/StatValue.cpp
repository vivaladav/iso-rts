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

void StatValue::SetValue(int val)
{
    // same value -> exit
    if(val == mData.d)
        return ;

    mData.d = val;

    NotifyObservers();
}

void StatValue::SetValue(float val)
{
    const float delta = 0.0001f;

    // almost same value -> exit
    if(std::fabs(mData.f - val) < delta)
        return ;

    mData.d = val;

    NotifyObservers();
}

void StatValue::SumValue(int val)
{
    mData.d += val;

    // clamp data
    if(mData.d < mMin.d)
        mData.d = mMin.d;
    else if(mData.d > mMax.d)
        mData.d = mMax.d;

    NotifyObservers();
}

void StatValue::SumValue(float val)
{
    mData.f += val;

    // clamp data
    if(mData.f < mMin.f)
        mData.f = mMin.f;
    else if(mData.f > mMax.f)
        mData.f = mMax.f;

    NotifyObservers();
}

void StatValue::NotifyObservers()
{
    for(auto & f : mCallbacks)
        f(this);
}

} // namespace game
