#include "utilities/windows/System.h"

#include <windows.h>

namespace lib
{
namespace utilities
{

bool System::OpenUrlInBrowser(const std::string & url)
{
    const int okThresh = 32;
    const int res = ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);

    return res > okThresh;
}

} // namespace utilities
} // namespace lib
