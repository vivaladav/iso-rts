#include "utilities/LoadedDie.h"

#include <cassert>
#include <chrono>

namespace lib
{
namespace utilities
{

LoadedDie::LoadedDie(const std::vector<float> & probabilities, int seed)
{
    InitDistribution(seed);

    // create event IDs and init thresholds
    const unsigned int numProbs = probabilities.size();

    mEventIds.reserve(numProbs);

    for(unsigned int i = 0; i < numProbs; ++i)
        mEventIds.emplace_back(i);

    InitTresholds(probabilities);
}

LoadedDie::LoadedDie(const std::vector<int> & eventIds,
                     const std::vector<float> & probabilities,
                     int seed)
    : mEventIds(eventIds)
{
    assert(eventIds.size() == probabilities.size());

    InitDistribution(seed);

    InitTresholds(probabilities);
}

int LoadedDie::GetNextValue()
{
    const float res = mDist(mGen);

    const unsigned int numProbs = mTresholds.size();

    for(unsigned int i = 0; i < numProbs; ++i)
    {
        if(res < mTresholds[i])
            return i;
    }

    // fallback solution in case last treshold is < PROB_MAX
    return numProbs - 1;
}

void LoadedDie::InitDistribution(int seed)
{
    if(SEED_RANDOM == seed)
        mGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    else
        mGen.seed(seed);

    mDist.param(std::uniform_real_distribution<float>::param_type(PROB_0, PROB_MAX));
}

void LoadedDie::InitTresholds(const std::vector<float> & probabilities)
{
    const unsigned int numProbs = probabilities.size();
    mTresholds.reserve(numProbs);

    // compute total of probabilities
    float totProbs = 0.f;

    for(float prob : probabilities)
        totProbs += prob;

    // convert probabilities to the range [PROB_MIN, PROB_MAX]
    for(float prob : probabilities)
        mTresholds.push_back(prob * PROB_MAX / totProbs);
}

} // namespace utilities
} // namespace lib
