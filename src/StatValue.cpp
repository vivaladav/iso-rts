#include "StatValue.h"

#include <cmath>

namespace game
{

void StatValue::SetValue(int val)
{
    // same value -> exit
    if(val == mData.d)
        return ;

    mData.d = val;

    // notify observers
    for(auto & f : mCallbacks)
        f(this);
}

void StatValue::SetValue(float val)
{
    const float delta = 0.0001f;

    // almost same value -> exit
    if(std::fabs(mData.f - val) < delta)
        return ;

    mData.d = val;

    // notify observers
    for(auto & f : mCallbacks)
        f(this);
}

} // namespace game
