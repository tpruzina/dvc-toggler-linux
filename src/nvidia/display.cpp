#include "display.hpp"

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/WinUtil.h>
#include <locale.h>

XDisplay::XDisplay()
{
	setlocale(LC_ALL, "");
	dpy = XOpenDisplay(NULL);
	if(!dpy)
		throw "Cannot open display";
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

// Query main PID of currently focused window
unsigned
XDisplay::query_focused_window_pid()
{
	static pid_t pid_old = 0;
	static Window focused_old=0;

	Window focused = query_focused_window((Display*)dpy);
	if(focused == focused_old)
		return pid_old;

	focused_old = focused;
	return (pid_old = query_window_pid((Display*)dpy,
					   query_name_window((Display*)dpy,
							     query_top_window((Display*)dpy,
									      focused))));
}

#ifdef DEBUG_XDISPLAY
int main()
{
	XDisplay *nv = new XDisplay;
	cout << "query_focused_window_pid(): " << nv->query_focused_window_pid() << endl;
	free(nv);
	return 0;
}
#endif // DEBUG_XDISPLAY
