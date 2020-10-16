#pragma once

// Random number distribution that produces integer values according to a uniform discrete distribution.

#include <random>

namespace lib
{
namespace utilities
{

class UniformDistribution
{
public:
    static const int SEED_RANDOM = 0;

public:
	// values will be generated in the range [min, max]
    UniformDistribution(int min, int max, int seed = SEED_RANDOM);

	void SetParameters(int min, int max);

	int GetNextValue();

private:
	std::uniform_int_distribution<int> mDistribution;
	std::mt19937 mGenerator;
};

// ===== INLINE FUNCTIONS =====
inline void UniformDistribution::SetParameters(int min, int max)
{
	mDistribution.param(std::uniform_int_distribution<int>::param_type(min, max));
}

inline int UniformDistribution::GetNextValue() { return mDistribution(mGenerator); }

} // namespace utilities
} // namespace lib
