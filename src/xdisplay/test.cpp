#include "xdisplay.hpp"

#include <iostream>

int main()
{
        auto pid = XDisplay::queryFocusedWindowPID();
        std::cout << "query_focused_window_pid(): " <<
                pid << '\n';
        return 0;
}
