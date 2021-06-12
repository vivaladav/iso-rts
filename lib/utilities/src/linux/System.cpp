#include "utilities/linux/System.h"

#include <cstdlib>
#include <string>

namespace lib
{
namespace utilities
{

bool System::OpenUrlInBrowser(const std::string & url)
{
    const unsigned int lenUrl = url.length();

    const std::string quote("\"");
    const unsigned int lenQuote = quote.length();

    std::string str;

    int res = 0;

    // TRY x-www-browser
    const std::string com1("x-www-browser \"");
    const unsigned int len1 = com1.length() + lenUrl + lenQuote;

    str.reserve(len1);
    str.append(com1).append(url).append(quote);

    res = system(str.c_str());

    // success -> exit
    if(0 == res)
        return true;

    // TRY xdg-open
    const std::string com2("xdg-open \"");
    const unsigned int len2 = com2.length() + lenUrl + lenQuote;

    str.clear();
    str.reserve(len2);
    str.append(com2).append(url).append(quote);

    res = system(str.c_str());

    // success -> exit
    if(0 == res)
        return true;

    // -- DESPERATE FALLBACK SOLUTIONS --

    // TRY google-chrome
    const std::string com3("google-chrome \"");
    const unsigned int len3 = com3.length() + lenUrl + lenQuote;

    str.clear();
    str.reserve(len3);
    str.append(com3).append(url).append(quote);

    res = system(str.c_str());

    // success -> exit
    if(0 == res)
        return true;

    // TRY firefox
    const std::string com4("firefox \"");
    const unsigned int len4 = com3.length() + lenUrl + lenQuote;

    str.clear();
    str.reserve(len4);
    str.append(com4).append(url).append(quote);

    res = system(str.c_str());

    return (0 == res);
}

} // namespace utilities
} // namespace lib
