#include "proc_watch.hpp"

ProcWatch::ProcWatch(bool active, unsigned sleep_ms) :
	active(active),
	terminate(false),
	sleep_ms(sleep_ms)
{
	watcher = std::thread(&ProcWatch::update, this);
}

ProcWatch::~ProcWatch()
{
   terminate = true;
   watcher.join();
}

bool
ProcWatch::is_proc_running(string proc_comm)
{
    // RAII lock for reads
    std::shared_lock < std::shared_mutex > lock(write);
    auto got = comms.find(proc_comm);
    if(got == comms.end())
	return false;
    else
	return true;
}

// return sorted, unique list of comms from /proc
vector < string > ProcWatch::list_running_procs()
{
	// scan /proc/PID/comms for list of viable targets
	comms = scan_proc();

	// RAII lock for reads
	std::shared_lock < std::shared_mutex > lock(write);

	// move elements of comms set into vector
	vector < string > res(comms.begin(), comms.end());
	// sort for user output
	std::sort(res.begin(), res.end());
	return res;
}

void ProcWatch::update()
{
	while (true)
	{
		// return worker if terminate flag is set
		if (terminate)
			break;

		// only update state / apply rules if we are active
		if(active)
		{
			// query currently focused window PID
			pid_t focused_window_pid = query_focused_window_pid();

			// update comm this has changed since the last update
			if(focused_window_pid != previous_active_window_pid)
				active_window_comm = pid_to_comm(focused_window_pid);
		}

		// sleep for sleep_ms
		std::this_thread::sleep_for(std::chrono::
					    milliseconds(sleep_ms));
	}
}

void
ProcWatch::set_enabled(bool state)
{
	active = state;
}

void
ProcWatch::set_polling_rate(unsigned ms)
{
    std::chrono::milliseconds cast_ms(ms);
    this->sleep_ms = cast_ms;
    if(ms > 500)
	std::cout << __FUNCTION__ << ": setting sleep time to " << ms << "ms will slow down object destructor" << std::endl;
}

void ProcWatch::debug_dump()
{
	std::shared_lock<std::shared_mutex> lock(write);
	for (auto & p:comms)
		std::cout << p << std::endl;
}

string
ProcWatch::pid_to_comm(pid_t pid)
{
	static char path[32] = "/proc/";
	const off_t offset = 6;	// strlen("/proc/_");
	sprintf(path+offset, "%d/comms%c", pid, '\0');

	return [] (const char *filename)
	{
	  std::ifstream in(filename, std::ios::in | std::ios::binary);
	  std::ostringstream contents;
	  contents << in.rdbuf();
	  in.close();
	  string str = contents.str();
	  str.pop_back(); // remove trailing \n
	  return (str);
	} (path);
}

// returns unique "comm" values from of /proc/$PID/comms for all $PIDS
unordered_set<string>
ProcWatch::scan_proc()
{
	DIR *dir;
	struct dirent *ent;
	unordered_set <string> set;

	// RAII lock for writer (unlocks itself after leaving scope)
	std::lock_guard < std::shared_mutex > lock(write);

	if (!(dir = opendir("/proc")))
		return set;

	static char path[32] = "/proc/";
	const off_t offset = 6;	// strlen("/proc/_");

	// for each ent=PID in /proc/<PIDs>
	while ((ent = readdir(dir))) {
		const char *pid_s = ent->d_name;
		if (!pid_s || pid_s[0] < '0' || pid_s[0] > '9')
			continue;

		strcpy((char *)(path + offset), pid_s);

		auto read_file_to_str = [] (const char *filename)
		{
		  std::ifstream in(filename, std::ios::in | std::ios::binary);
		  std::ostringstream contents;
		  contents << in.rdbuf();
		  in.close();
		  string str = contents.str();
		  str.pop_back(); // remove trailing \n
		  return (str);
		};

//		// check maps and skip adding to set if there is no libX11.so present
//		// TODO: cache results instead of parsing the whole thing on every refresh
		strcpy((char *)(path + offset + strlen(pid_s)), "/maps");
		string maps = read_file_to_str(path);
		size_t found = maps.find("libX11.so");
		if(found == std::string::npos)
			continue;

		strcpy((char *)(path + offset + strlen(pid_s)), "/comm");
		string comm = read_file_to_str(path);
		set.insert(comm);
	}
	closedir(dir);
	return set;
}
