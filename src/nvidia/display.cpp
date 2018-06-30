#include "display.hpp"

XDisplay::XDisplay()
{
	dpy = XOpenDisplay(NULL);
	if(!dpy)
	{
		cerr << "Cannot open display " << XDisplayName(NULL) << endl;
	}
}

XDisplay::~XDisplay()
{
	if(this->dpy)
		XCloseDisplay(this->dpy);
}

string
XDisplay::query_active_window_name()
{
	string name;
	Window test;
	int tmp;

	if(dpy)
	{
		XGetInputFocus(dpy, &test, &tmp);
		XClassHint wm_class;
		if(!XGetClassHint(dpy, test, &wm_class))
			return name;

		name.assign(wm_class.res_name);

		// cleanup Xlib strings
		XFree(wm_class.res_name);
		XFree(wm_class.res_class);
	}

	return name;
}

Window
XDisplay::query_top_window(Window start)
{
	Window w = start;
	Window parent = start;
	Window root = None;
	Window *children;
	unsigned int nchildren;
	Status s;

	while (parent != root)
	{
		w = parent;
		s = XQueryTree(dpy, w, &root, &parent, &children, &nchildren);    // see man
		if (s)
			XFree(children);

	}
	return w;
}

// finds a window, at or below the specified window, which has a WM_STATE property
Window
XDisplay::query_name_window(Window start)
{
	return XmuClientWindow(dpy, start);
}

int
XDisplay::query_active_window_pid()
{
    uint32_t pid(0);
    Window w;
    int tmp;
    Atom am_wm_pid;

    if(!dpy)
        return 0;

    XGetInputFocus(dpy, &w, &tmp);
    am_wm_pid = XInternAtom(dpy, "_NET_WM_PID", False);
    Atom type;
    int format;
    unsigned long nitems, bytes;
    unsigned char *prop;
    int status;

    status = XGetWindowProperty(dpy,
                                w,
                                am_wm_pid,
                                0,
                                1024,
                                false,
                                XA_CARDINAL,
                                &type,
                                &format,
                                &nitems,
                                &bytes,
                                &prop
    );

    if(status == 0 && nitems != 0)
    {
        pid = prop[0] | prop[1] << 8 | prop[2] << 16 | prop[3] << 24;
        XFree(prop);
    }

    return pid;
}

int
XDisplay::query_default_screen()
{
	return DefaultScreen(dpy);
}


string
XDisplay::query_window_class(Window w)
{
	string ret;
	XClassHint *clh = XAllocClassHint();
	if(!clh)
		return ret;
	Status s = XGetClassHint(dpy, w, clh);
	if(s)
		ret = clh->res_class;
	XFree(clh);
	return ret;
}

string
XDisplay::query_window_name(Window w)
{
	string ret;
	XTextProperty prop;
	Status s = XGetWMName(dpy, w, &prop);
	if(s)
	{
		int count;
		char **list = NULL;
		if(Success == XmbTextPropertyToTextList(dpy, &prop, &list, &count))
			ret = list[0];
	}
	return ret;
}
