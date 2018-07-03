#ifndef PROCWATCH_HPP
#define PROCWATCH_HPP

#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <unordered_set>
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

typedef int pid_t;
using std::unordered_set;
using std::string;
using std::vector;

class ProcWatch : public XDisplay{
 public:
	ProcWatch(bool active = true, unsigned sleep_ms=100);
	~ProcWatch();

	vector<string> list_running_procs();
	bool is_proc_running(string proc_comm);

	void debug_dump();

	void set_polling_rate(unsigned ms);
	void set_enabled(bool state = true);

 private:
	void update();
	unordered_set <string> scan_proc();
	string pid_to_comm(pid_t pid);



	unordered_set <string> comms;
	pid_t previous_active_window_pid;
	string active_window_comm;

	volatile bool active;
	volatile bool terminate;
	std::chrono::milliseconds sleep_ms;

	std::shared_mutex write;
	std::thread watcher;
};

#endif // PROCWATCH_H
