#pragma once

#if defined(LINUX)
    #include "utilities/linux/System.h"
#elif defined(WINDOWS)
    #include "utilities/windows/System.h"
#else
    #error Platform not supported
#endif
