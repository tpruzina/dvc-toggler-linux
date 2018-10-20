#include <chrono>
#include <thread>

#include "nvidia.hpp"

auto main(int argc, char **argv) noexcept -> int
{
        if(!NVIDIA::isAvailable())
                return 1;

        auto res = NVIDIA::getVibrance();
        auto prev = res;

        for(auto i=-100; i <= 100; i += 50)
        {
                for(auto &scr: res)
                        scr.second = i;
                
                NVIDIA::setVibrance(res);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        NVIDIA::setVibrance(prev);

        return 0;
}
