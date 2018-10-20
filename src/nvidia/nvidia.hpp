#ifndef COMMON_NVIDIA_HPP_
#define COMMON_NVIDIA_HPP_

#include <map>
#define NVIDIA_APPLY_TO_ALL -1

using DVCMap = std::map<int, int>;

class NVIDIA
{
public:
        auto static isAvailable() noexcept -> bool;
        auto static getVibrance() noexcept -> DVCMap;
        auto static setVibrance(DVCMap &values) noexcept -> void;
};
#endif // COMMON_NVIDIA_HPP_
