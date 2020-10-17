#pragma once

#include <random>
#include <vector>

namespace lib
{
namespace utilities
{

// An uneven die which generates random events according to associated probabilities
class LoadedDie
{
public:
    static constexpr int SEED_RANDOM = 0;

public:
    LoadedDie(const std::vector<float> & probabilities, int seed = SEED_RANDOM);
    LoadedDie(const std::vector<int> & eventIds,
              const std::vector<float> & probabilities,
              int seed = SEED_RANDOM);

    int GetNextValue();

private:
    void InitDistribution(int seed);

    void InitTresholds(const std::vector<float> & probabilities);

private:
    static constexpr float PROB_0 = 0.f;
    static constexpr float PROB_MAX = 100.f;

private:
    std::vector<float> mTresholds;
    std::vector<int> mEventIds;

    std::uniform_real_distribution<float> mDist;
    std::mt19937 mGen;
};

} // namespace utilities
} // namespace lib
