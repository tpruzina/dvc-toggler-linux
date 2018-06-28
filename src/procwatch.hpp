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

typedef int pid_t;
using std::unordered_set;
using std::string;
using std::vector;

class ProcWatch {
 public:
	ProcWatch();
	~ProcWatch();

	vector < string > list_running_procs();
	bool is_proc_running(string proc_comm);
	void debug_dump();

	void set_polling_rate(unsigned ms);

 private:
	void update();
	static unordered_set < string > scan_proc();
	std::chrono::milliseconds sleep_ms;
	unordered_set < string > comms;

	volatile bool terminate;
	std::shared_mutex write;
	std::thread watcher;
};

#endif // PROCWATCH_H
