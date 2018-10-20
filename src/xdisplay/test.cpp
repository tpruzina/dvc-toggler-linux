#include "xdisplay.hpp"

#include <iostream>

int main()
{
        XDisplay nv;
	std::cout << "query_focused_window_pid(): " <<
		nv.queryFocusedWindowPID() << '\n';
        return 0;
}
