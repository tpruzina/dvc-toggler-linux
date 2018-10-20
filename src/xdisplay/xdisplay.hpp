#ifndef X_DISPLAY_HPP_
#define X_DISPLAY_HPP_

#include <sys/types.h>

class XDisplay
{
public:  
        XDisplay() = delete;
        auto static queryFocusedWindowPID() noexcept -> pid_t;
};

#endif // X_DISPLAY_HPP_
