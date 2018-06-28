#ifndef COMMON_DISPLAY_HPP_
#define COMMON_DISPLAY_HPP_

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

class DISPLAY
{
public:
	DISPLAY();
	~DISPLAY();

	string query_active_window_name();
	int	query_default_screen();

private:
	Display *dpy;

	friend class MOUSE;
	friend class NVIDIA;
	friend class APP;
};




#endif /* COMMON_DISPLAY_HPP_ */
