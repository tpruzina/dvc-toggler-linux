#ifndef COMMON_NVIDIA_HPP_
#define COMMON_NVIDIA_HPP_

#include <inttypes.h>
#include "libXNVCtrl/NVCtrl.h"
#include "libXNVCtrl/NVCtrlLib.h"
#include "libXNVCtrl/nv-control-screen.h"
#include "display.hpp"

#define NVIDIA_APPLY_TO_ALL -1

class NVIDIA : public DISPLAY
{
public:
	int get_vibrance();
	int set_vibrance(int level, int chosen_dpy);
        int set_vibrance(int level) { return set_vibrance(level, NVIDIA_APPLY_TO_ALL); }

	int get_core_freq_offset();
	int set_core_freq_offset(int offset);
private:
        int level;
};

#endif /* COMMON_NVIDIA_HPP_ */
