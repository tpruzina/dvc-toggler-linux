#ifndef DBUS_HPP
#define DBUS_HPP

#include <chrono>
#include <thread>

#define DVC_DBUS_CLIENT_NAME "dvc.signal.client"
#define DVC_DBUS_HOST_SERVER "dvc.signal.server"
#define DVC_DBUS_IF_NAME "dvc.signal.Type"
#define DVC_DBUS_SIGNAL_OBJECT "/dvc/signal/Object"
#define DVC_DBUS_SIGNAL_SHOW "Show"

class DBusInterface {
public:
	DBusInterface() : shutdown(false), sleep_ms(100) {}

	~DBusInterface()
	{
		if(listener.joinable())
		{
			shutdown = true;
			listener.join();
		}
	}

	void spawnListener(void (*callback_fn)(void*), void* that);
	void receive();
	void sendSignal(char *message);

private:
	std::thread listener;
	void (*callback_fn) (void *);
	void *callback_object;
	volatile bool shutdown;
	std::chrono::milliseconds sleep_ms;
};
#endif	// DBUS_HPP
