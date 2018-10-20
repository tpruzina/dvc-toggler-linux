#include "xdisplay.hpp"

#include <iostream>

int main()
{
        auto pid = XDisplay::queryFocusedWindowPID();
        if(pid <= 0)
        {
                std::cerr << "query_focused_window_pid() returned "
                        << pid 
                        << " (this should only happen if "
                        << "there is no focused window or X isn't running)\n";
                return -1;
        }
        else
        {
                std::cout << "query_focused_window_pid(): " <<
                pid << '\n';
        }
        return 0;
}
