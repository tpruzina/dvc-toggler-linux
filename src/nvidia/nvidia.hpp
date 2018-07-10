#ifndef COMMON_NVIDIA_HPP_
#define COMMON_NVIDIA_HPP_

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
	static std::map<int, int> getVibrance();
	static int setVibrance(std::map<int, int> *values);
	static bool isScreenAvailable();
};

#endif // COMMON_NVIDIA_HPP_
