#include "utilities/UniformDistribution.h"

#include <chrono>

namespace lib
{
namespace utilities
{

UniformDistribution::UniformDistribution(int min, int max, int seed)
{
    if(SEED_RANDOM == seed)
        mGenerator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	else
        mGenerator.seed(seed);

	mDistribution.param(std::uniform_int_distribution<int>::param_type(min, max));
}

} // namespace utilities
} // namespace lib
