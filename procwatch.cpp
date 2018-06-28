#include "procwatch.hpp"

ProcWatch::ProcWatch() :
    sleep_ms(100),
    terminate(false)
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

		// RAII lock for writer (unlocks itself after leaving scope)
		{
		    std::lock_guard < std::shared_mutex > lock(write);
		    // do the proc read and update set
		    this->comms = scan_proc();
		    // sleep for sleep_ms
		}
		std::this_thread::sleep_for(std::chrono::
					    milliseconds(sleep_ms));
	}
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

// returns unique "comm" values from of /proc/$PID/comms for all $PIDS
unordered_set < string > ProcWatch::scan_proc()
{
	DIR *dir;
	struct dirent *ent;
	unordered_set < string > set;

	if (!(dir = opendir("/proc")))
		return set;

	static char name_buffer[256] = "/proc/";
	const off_t offset = 6;	// strlen("/proc/_");

	// for each ent=PID in /proc/<PIDs>
	while ((ent = readdir(dir))) {
		const char *pid_s = ent->d_name;
		if (!pid_s || pid_s[0] < '0' || pid_s[0] > '9')
			continue;

		strcpy((char *)(name_buffer + offset), pid_s);
		strcpy((char *)(name_buffer + offset + strlen(pid_s)), "/comm");

		set.insert( // insert contents of /proc/$pid/comm
				  [] (const char *filename)
				  {
				    std::ifstream in(filename, std::ios::in | std::ios::binary);
				    std::ostringstream contents;
				    contents << in.rdbuf();
				    in.close();
				    string str = contents.str();
				    str.pop_back(); // remove trailing \n
				    return (str);
				  }
				  (name_buffer)
		);
	}
	closedir(dir);
	return set;
}
