#include <functional>
#include <unordered_map>

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

    unsigned int AddOnValueChanged(const std::function<void(const StatValue *)> & f);
    unsigned int AddOnRangeChanged(const std::function<void(const StatValue *)> & f);
    void RemoveOnValueChanged(unsigned int funId);
    void RemoveOnRangeChanged(unsigned int funId);
    void ClearCallbacks();

private:
    void NotifyObserversValue();
    void NotifyObserversRange();

private:
    std::unordered_map<unsigned int, std::function<void(const StatValue *)>> mCallbacksVal;
    std::unordered_map<unsigned int, std::function<void(const StatValue *)>> mCallbacksRange;

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

inline int StatValue::GetIntValue() const { return mData.d; }
inline float StatValue::GetFloatValue() const { return mData.f; }

inline void StatValue::ClearCallbacks()
{
    mCallbacksVal.clear();
    mCallbacksRange.clear();
}

} // namespace game
