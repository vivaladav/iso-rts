#pragma once

#include "utilities/ISystem.h"

namespace lib
{
namespace utilities
{

class System : public ISystem
{
public:
    bool OpenUrlInBrowser(const std::string & url) override;
};

} // namespace utilities
} // namespace lib
