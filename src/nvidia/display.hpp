#ifndef COMMON_DISPLAY_HPP_
#define COMMON_DISPLAY_HPP_

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/WinUtil.h>
#include <locale.h>

#undef Bool
#undef CursorShape
#undef Expose
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef None
#undef Status
#undef Unsorted

using std::string;
using std::cout;
using std::cerr;
using std::endl;

class XDisplay
{
public:
	XDisplay();
	~XDisplay();

	string	query_active_window_name();
	int     query_active_window_pid();
        int	query_default_screen();

	Window	query_focused_window();
	Window	query_top_window(Window start);
	Window	query_name_window(Window start);

	string	query_window_class(Window w);
	string	query_window_name(Window w);
	pid_t	query_window_pid(Window w);


private:
	Display *dpy;

	friend class MOUSE;
	friend class NVIDIA;
	friend class APP;

	int32_t get_prop_card32(Window w, Atom p);
};

#endif /* COMMON_DISPLAY_HPP_ */
