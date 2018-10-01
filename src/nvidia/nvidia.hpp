#ifndef COMMON_NVIDIA_HPP_
#define COMMON_NVIDIA_HPP_

#include <map>

#define NVIDIA_APPLY_TO_ALL -1

// Nvidia DVC setter
// TODO:        create C++ interface for this to support non-NVIDIA cards
//                Intel/AMD has basically similar functionality (saturation)
// TODO:        Remove ugly workarounds around Xlib<>QT symbol mangling (is namespace sufficient for this??)
class NVIDIA
{
public:
        NVIDIA() noexcept;
        ~NVIDIA() noexcept;
        std::map<int, int> getVibrance() noexcept;
        int setVibrance(std::map<int, int> &values) noexcept;
        static bool isScreenAvailable() noexcept;

private:
        void *dpy;
        int screen;
        std::map<int, int> prev;
};
#endif // COMMON_NVIDIA_HPP_
