#ifndef COMMON_NVIDIA_HPP_
#define COMMON_NVIDIA_HPP_

#include <inttypes.h>
#include "libXNVCtrl/NVCtrl.h"
#include "libXNVCtrl/NVCtrlLib.h"
#include "libXNVCtrl/nv-control-screen.h"

#include <map>
#include <vector>
#include <algorithm> // std::transform
#include <iostream>

#define NVIDIA_APPLY_TO_ALL -1

// Nvidia DVC setter
// TODO:	create C++ interface for this to support non-NVIDIA cards
//		Intel/AMD has basically similar functionality (saturation)
// TODO:	Remove ugly workarounds around Xlib<>QT symbol mangling (is namespace sufficient for this??)
class NVIDIA
{
public:
	NVIDIA();
	std::map<int, int> get_vibrance();
	int set_vibrance(std::map<int, int> *values);
private:
	Display *dpy;
};

#endif /* COMMON_NVIDIA_HPP_ */
