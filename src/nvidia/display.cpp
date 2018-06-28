#include "display.hpp"

DISPLAY::DISPLAY()
{
	dpy = XOpenDisplay(NULL);
	if(!dpy)
	{
		cerr << "Cannot open display " << XDisplayName(NULL) << endl;
	}
}

DISPLAY::~DISPLAY()
{
	if(this->dpy)
		XCloseDisplay(this->dpy);
}

string
DISPLAY::query_active_window_name()
{
	string name;
	Window test;
	int tmp;

	if(dpy)
	{
		cout << XDisplayName(NULL) << endl;
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

int
DISPLAY::query_default_screen()
{
	return DefaultScreen(dpy);
}
