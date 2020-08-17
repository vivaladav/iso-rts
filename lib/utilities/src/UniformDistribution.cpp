#include "utilities/UniformDistribution.h"

#include <chrono>

namespace lib
{
namespace utilities
{

UniformDistribution::UniformDistribution(int min, int max, int seed)
{
	if(seed)
		mGenerator.seed(seed);
	else
		mGenerator.seed(std::chrono::system_clock::now().time_since_epoch().count());

	mDistribution.param(std::uniform_int_distribution<int>::param_type(min, max));
}

} // namespace utilities
} // namespace lib
