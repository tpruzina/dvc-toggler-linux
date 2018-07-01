#include "display.hpp"

XDisplay::XDisplay()
{
	setlocale(LC_ALL, "");
	dpy = XOpenDisplay(NULL);
	if(!dpy)
	{
		cerr << "Cannot open display " << XDisplayName(NULL) << endl;
		exit(1);
	}
}

XDisplay::~XDisplay()
{
	if(this->dpy)
		XCloseDisplay(this->dpy);
}

Window
XDisplay::query_focused_window()
{
	Window w;
	int tmp;
	XGetInputFocus(dpy, &w, &tmp);
	return w;
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
    Window w;
    w = XmuClientWindow(dpy, start);
    return w;
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

// @private:
// get 32bit sized window property using atom ("_NET_WM_PID", ...)
int32_t
XDisplay::get_prop_card32(Window w, Atom p)
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


pid_t
XDisplay::query_window_pid(Window w)
{
	Atom am_wm_pid = XInternAtom(dpy, "_NET_WM_PID", False);
	return get_prop_card32(w, am_wm_pid);
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

#ifdef DEBUG_XDISPLAY
int main()
{
    XDisplay *nv = new XDisplay;
    cout << "active window name: " << nv->query_active_window_name() << endl;
    cout << "active window pid: " << nv->query_active_window_pid() << endl;
    cout << "default screen: " << nv->query_default_screen() << endl;

    Window focused, top, name;

    focused = nv->query_focused_window();
    cout << "focused window class: " << nv->query_window_class(focused) << endl;
    cout << "focused PID: " << nv->query_window_pid(focused) << endl;

    top = nv->query_top_window(focused);
    cout << "focused->top class: " << nv->query_window_class(top) << endl;
    cout << "focused->top PID: " << nv->query_window_pid(top) << endl;

    name = nv->query_name_window(top);
    cout << "focused->top->name class: " << nv->query_window_class(name) << endl;
    cout << "focused->top->name PID: " << nv->query_window_pid(name) << endl;
    
    free(nv);
    return 0;
}
#endif // DEBUG_XDISPLAY
