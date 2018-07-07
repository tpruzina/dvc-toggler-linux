#ifndef DBUS_HPP
#define DBUS_HPP

#include <chrono>
#include <thread>

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

	void spawn_listener(void (*callback_fn)(void*), void* that);
	void receive();
	void sendsignal(char *message);

 private:
	std::thread listener;
	void (*callback_fn) (void *);
	void *callback_object;
	volatile bool shutdown;
	std::chrono::milliseconds sleep_ms;
};

#endif				// DBUS_HPP
