#ifndef COMMON_DISPLAY_HPP_
#define COMMON_DISPLAY_HPP_

// This intentionally doesn't use any includes from Xlib,
// because dealing with symbol mangling is pain
class XDisplay
{
public:
	XDisplay();
	~XDisplay();

	unsigned   query_focused_window_pid();
private:
	void *dpy;
};

#endif /* COMMON_DISPLAY_HPP_ */
