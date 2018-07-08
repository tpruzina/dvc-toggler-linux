#include "proc_watch.hpp"

// Process watcher
// Scans /proc/$PID/{comms, maps} and queries X server for running processes
// Applies DVC rules according to rules
// TODO: Get rid of STL & use C primitives, performance actually matters here
ProcWatch::ProcWatch(NVIDIA &nv, bool enabled, unsigned sleep_ms) :
	nv(nv),	// Nvidia DVC setting
	dirty(false),	// signals watcher that rules need to be reapplied
	active(enabled),	// do stuff?
	terminate(false),	// tells watcher thread that it should shut down
	sleep_ms(sleep_ms)	// sleep timer for watcher thread
{
	watcher = std::thread(&ProcWatch::update, this);
}

ProcWatch::~ProcWatch()
{
	terminate = true;
	watcher.join();
}

// return sorted, unique list of comms from /proc
vector <string>
ProcWatch::listRunningProcs()
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

// query whether proc is still running
// FIXME: is this still necessary?
bool
ProcWatch::isProcRunning(string proc_comm)
{
	// RAII lock for reads
	std::shared_lock < std::shared_mutex > lock(write);
	auto got = comms.find(proc_comm);
	if(got == comms.end())
		return false;
	else
		return true;
}

void
ProcWatch::updateRule(string name, std::map<int,int> dvc_map)
{
	// {comm}x{{dpyId}x{dvc}}

	//update/add rule
	rules[name] = dvc_map;

	// set dirty flag if window is focused
	if(name.compare(active_window_comm) == 0)
		dirty = true;
}

void
ProcWatch::applyRule(string &name)
{
	// find rule
	const auto &rule = rules.find(name);

	// if rule for app doesn't exist, apply "default"
	if(rule == rules.end())
		nv.setVibrance(&rules[CONFIG_DEFAULT_PROFILE_STR]);
	else	// else apply selected rule
		nv.setVibrance(&rules[name]);

	// unset dirty flag
	dirty = false;
}

void
ProcWatch::removeRule(string name)
{
	std::lock_guard<std::shared_mutex> lock(write);
	rules.erase(name);
	dirty = true;
}

// general update function, watcher thread runs this every sleep.ms
void
ProcWatch::update()
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
			pid_t focused_window_pid = queryFocusedWindowPID();

			// if focus has changed
			if(focused_window_pid != previous_active_window_pid)
			{
				// update comm this has changed since the last update
				active_window_comm = pid_to_comm(focused_window_pid);

				std::cerr << "PID=" << focused_window_pid << " COMM=" << active_window_comm << "\n";

				dirty = true;
				previous_active_window_pid = focused_window_pid;
			}

			if(dirty)
				applyRule(active_window_comm);
		}
		// go back to sleep for sleep_ms
		std::this_thread::sleep_for(std::chrono::
					    milliseconds(sleep_ms));
	}
}

void
ProcWatch::setEnabled(bool state)
{
	// tells watcher thread whether to do anything
	active = state;
}

void
ProcWatch::setPollingRate(unsigned ms)
{
	// sets polling rate for watcher process
	// TODO: add UI element/Config setting in mainWindow to control this
	std::chrono::milliseconds cast_ms(ms);
	this->sleep_ms = cast_ms;
	if(ms > 500)
		std::cout << __FUNCTION__ << ": setting sleep time to " << ms << "ms will slow down object destructor" << std::endl;
}

// read /proc/$PID/comm name given $PID$
string
ProcWatch::pid_to_comm(pid_t pid)
{
	static char path[32] = "/proc/";
	const off_t offset = 6;	// strlen("/proc/_");
	sprintf(path+offset, "%d/comm%c", pid, '\0');

	return [] (const char *filename)
	{
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		string str = contents.str();
		if(!str.empty())
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
			if(!str.empty())
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
