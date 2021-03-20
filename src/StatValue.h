#include <functional>
#include <vector>

#pragma once

namespace game
{

class StatValue
{
public:
    StatValue(unsigned int statId, int val);
    StatValue(unsigned int statId, float val);

    unsigned int GetId() const;

    int GetIntMin() const;
    float GetFloatMin() const;
    int GetIntMax() const;
    float GetFloatMax() const;

    void SetMin(int min);
    void SetMin(float min);
    void SetMax(int max);
    void SetMax(float max);

    int GetIntValue() const;
    float GetFloatValue() const;

    void SetValue(int val);
    void SetValue(float val);

    void SumValue(int val);
    void SumValue(float val);

    void AddOnValueChanged(const std::function<void(const StatValue *)> & f);
    void ClearCallbacks();

private:
    void NotifyObservers();

private:
    std::vector<std::function<void(const StatValue *)>> mCallbacks;

    unsigned int mId = 0;

    union DataPoint
    {
        DataPoint(int v) : d(v) {}
        DataPoint(float v) : f(v) {}

        int d;
        float f;
    };

    DataPoint mData;

    DataPoint mMin;
    DataPoint mMax;
};

inline unsigned int StatValue::GetId() const { return mId; }

inline int StatValue::GetIntMin() const { return mMin.d; }
inline float StatValue::GetFloatMin() const { return mMin.f; }
inline int StatValue::GetIntMax() const { return mMax.d; }
inline float StatValue::GetFloatMax() const { return mMax.f; }

inline void StatValue::SetMin(int min)
{
    mMin.d = min;

    if(mData.d < mMin.d)
        mData.d = mMin.d;
}

inline void StatValue::SetMin(float min)
{
    mMin.f = min;

    if(mData.f < mMin.f)
        mData.f = mMin.f;
}

inline void StatValue::SetMax(int max)
{
    mMax.d = max;

    if(mData.d > mMax.d)
        mData.d = mMax.d;
}

inline void StatValue::SetMax(float max)
{
    mMax.f = max;

    if(mData.f > mMax.f)
        mData.f = mMax.f;
}

inline int StatValue::GetIntValue() const { return mData.d; }
inline float StatValue::GetFloatValue() const { return mData.f; }

inline void StatValue::AddOnValueChanged(const std::function<void(const StatValue *)> & f)
{
    mCallbacks.push_back(f);
}

inline void StatValue::ClearCallbacks() { mCallbacks.clear(); }

} // namespace game
