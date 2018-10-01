#ifndef X_DISPLAY_HPP_
#define X_DISPLAY_HPP_

// This intentionally doesn't use any includes from Xlib,
// because dealing with symbol mangling is pain

class XDisplay
{
public:
        XDisplay() noexcept;
        ~XDisplay() noexcept;

        pid_t   queryFocusedWindowPID() noexcept;
private:
        void *dpy;
};

#endif // X_DISPLAY_HPP_
