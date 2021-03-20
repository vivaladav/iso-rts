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

    int GetIntValue() const;
    float GetFloatValue() const;

    void SetValue(int val);
    void SetValue(float val);

    void AddOnValueChanged(const std::function<void(const StatValue *)> & f);
    void ClearCallbacks();

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
};

inline StatValue::StatValue(unsigned int statId, int val) : mId(statId), mData(val) { }
inline StatValue::StatValue(unsigned int statId, float val) : mId(statId), mData(val) { }

inline unsigned int StatValue::GetId() const { return mId; }

inline int StatValue::GetIntValue() const { return mData.d; }
inline float StatValue::GetFloatValue() const { return mData.f; }

inline void StatValue::AddOnValueChanged(const std::function<void(const StatValue *)> & f)
{
    mCallbacks.push_back(f);
}

inline void StatValue::ClearCallbacks() { mCallbacks.clear(); }

} // namespace game
