#include "run_guard.hpp"

#include <iostream>

auto main() -> int
{
        RunGuard guard("run_guard_test");
        
        if(!guard.tryToRun())
        {
                std::cerr << __FILE__ << ':' << __LINE__
                        << ": failed to init run guard!\n";
                return -1;
        }

        if(!guard.tryToRun())
        {
                std::cerr << __FILE__ << ':' << __LINE__
                        << ": run guard should be up!\n";
        }

        // release
        guard.release();

        // check and aquire
        if(!guard.tryToRun())
        {
                std::cerr << __FILE__ << ':' << __LINE__
                        << ":run guard should be down!\n";
                return -1;
        }

        return 0;
}
