#ifndef PROCWATCH_HPP
#define PROCWATCH_HPP

#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>

#include <thread>
#include <shared_mutex>
#include <mutex>
#include <chrono>

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <cstring>

#include "nvidia/nvidia.hpp"
#include "xdisplay.hpp"
#include "config.hpp"

typedef int pid_t;
using std::unordered_set;
using std::string;
using std::vector;
using ms = std::chrono::milliseconds;

class ProcWatch : public XDisplay
{
public:
        ProcWatch(NVIDIA &nv, bool enabled=true, unsigned sleep_ms=100) noexcept;
        ~ProcWatch() noexcept;

        vector<string>        listRunningProcs() noexcept;
        bool isProcRunning(string proc_comm) noexcept;

        void updateRule(string name, std::map<int,int> &dvc_map) noexcept;
        void applyRule(string &name) noexcept;
        void removeRule(string name) noexcept;

        void setEnabled(bool state = true) noexcept;
        void setPollingRate(unsigned ms) noexcept;

private:
        // ref to nvidia object, applies dvc rules
        NVIDIA &nv;

        // watcher thread f()
        void update() noexcept;

        // returns set of running process names (comm)
        unordered_set <string> scan_proc() noexcept;

        // make this lambda, we have single user
        static string pid_to_comm(pid_t pid) noexcept;

        // {comm}x{{dpyId}x{dvc}}
        std::unordered_map<string,std::map<int,int>> rules;

        // set of running process comms (process name string)
        unordered_set <string> comms;

        // keep track of (previously) focused window pid/name
        pid_t previous_active_window_pid;
        string active_window_comm;

        bool dirty;        // rules need to be reapplied
        bool active;        // active/paused
        bool terminate; // signal watcher process to shutdown
        ms sleep_ms;        // how long does watcher process sleep in between updates

        std::shared_mutex write;        // readers/writers lock
        std::thread watcher;                // update thread
};

#endif // PROCWATCH_HPP
