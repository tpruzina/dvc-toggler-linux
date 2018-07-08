#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/WinUtil.h>
#include <locale.h>

#include "xdisplay.hpp"

XDisplay::XDisplay()
{
	setlocale(LC_ALL, "");
	dpy = XOpenDisplay(NULL);
	if(!dpy)
		throw "Cannot open display";	// FIXME: handle in mainWindow?
}

XDisplay::~XDisplay()
{
	if(dpy)
		XCloseDisplay((Display*)dpy);
}

static inline
Window
query_focused_window(Display *dpy)
{
	Window focused_window;
	int tmp;
	XGetInputFocus(dpy, &focused_window, &tmp);
	return focused_window;
}

static inline
Window
query_top_window(Display *dpy, Window start)
{
	Window w = start;
	Window parent = start;
	Window root = 0L; // = None (None from Xlib)
	Window *children;
	unsigned int nchildren;
	int s;

	while (parent != root)
	{
		w = parent;
		s = XQueryTree(dpy, w, &root, &parent, &children, &nchildren);
		if (s)
			XFree(children);

	}
	return w;
}

//// finds a window, at or below the specified window, which has a WM_STATE property
static inline
Window
query_name_window(Display *dpy, Window start)
{
	Window w;
	w = XmuClientWindow(dpy, start);
	return w;
}

// @private:
// get 32bit sized window property using atom ("_NET_WM_PID", ...)
static inline
int32_t
get_prop_card32(Display *dpy, Window w, Atom p)
{
	Atom actual_type;
	int actual_format;
	unsigned long nitems, bytes;
	unsigned char *prop;
	int32_t status, value;

	status = XGetWindowProperty(dpy, w, p,
				    /* long_offset */ 0L,
				    /* long_length */ 1024L,
				    /* delete */ False,
				    /* req_type */ XA_CARDINAL,
				    /* actual_type_return */ &actual_type,
				    /* actual_format_return */ &actual_format,
				    /* nitems_return */ &nitems,
				    /* bytes_after_return */ &bytes,
				    /* prop_return */ &prop);

	if (status != 0 || nitems < 1)
		return -1;

	value = prop[0] | prop[1] << 8 | prop[2] << 16 | prop[3] << 24;
	XFree(prop);
	return value;
}

static inline
pid_t
query_window_pid(Display *dpy, Window w)
{
	Atom am_wm_pid = XInternAtom(dpy, "_NET_WM_PID", False);
	return get_prop_card32(dpy, w, am_wm_pid);
}

// Query main PID (_NET_WM_PID) of currently focused window
// Focused window doesn't necessarily has this attribute,
// we need to go from focused window -> top window -> name window(_NET_WM_PID)
unsigned
XDisplay::queryFocusedWindowPID()
{
	// save previous values in static variables in order to cache results
	// as much as possible and prevent expensive X queries
	static pid_t pid_old = 0;
	static Window prev_focused, prev_top, prev_name;

	Window focused = query_focused_window((Display*)dpy);
	if(focused == prev_focused)
		return pid_old;
	else
		prev_focused = focused;

	Window top = query_top_window((Display*)dpy, focused);
	if(top == prev_top)
		return pid_old;
	else
		prev_top = top;

	Window name = query_name_window((Display*)dpy, top);
	if(name == prev_name)
		return pid_old;
	else
		prev_name = name;

	pid_old = query_window_pid((Display*)dpy, name);
	return pid_old;
}

#ifdef DEBUG_XDISPLAY
int main()
{
	XDisplay nv;
	cout << "query_focused_window_pid(): " << nv.query_focused_window_pid() << endl;
	return 0;
}
#endif // DEBUG_XDISPLAY