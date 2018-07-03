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

#include "nvidia/display.hpp"
#include "nvidia/nvidia.hpp"

typedef int pid_t;
using std::unordered_set;
using std::string;
using std::vector;

class ProcWatch : public XDisplay
{
public:
	ProcWatch(NVIDIA &nv, bool active = true, unsigned sleep_ms=100);
	~ProcWatch();

	vector<string>	list_running_procs();
	bool is_proc_running(string proc_comm);	// is this necessary?

	void update_rule(string name, std::map<int,int> dvc_map);
	void apply_rule(string &name);
	void remove_rule(string name);

	void set_enabled(bool state = true);
	void set_polling_rate(unsigned ms);

	void debug_dump();

private:
	// ref to nvidia object, applies dvc rules
	NVIDIA &nv;

	// watcher thread f()
	void update();

	// returns set of running process names (comm)
	unordered_set <string> scan_proc();

	// make this lambda, we have single user
	string pid_to_comm(pid_t pid);

	// {comm}x{{dpyId}x{dvc}}
	std::unordered_map<string,std::map<int,int>> rules;

	// set of running process comms (process name string)
	unordered_set <string> comms;

	// keep track of (previously) focused window pid/name
	pid_t previous_active_window_pid;
	string active_window_comm;

	volatile bool dirty;	// rules need to be reapplied
	volatile bool active;	// active/paused
	volatile bool terminate; // signal watcher process to shutdown
	std::chrono::milliseconds sleep_ms;	// how long does watcher process sleep in between updates

	std::shared_mutex write;	// readers/writers lock
	std::thread watcher;		// update thread
};

#endif // PROCWATCH_H
