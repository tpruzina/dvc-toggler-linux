#ifndef DBUS_HPP
#define DBUS_HPP

#include <chrono>
#include <thread>

#define DBUS_CLIENT_NAME "dvc.signal.client"
#define DBUS_HOST_SERVER "dvc.signal.server"
#define DBUS_IF_NAME "dvc.signal.Type"
#define DBUS_SIGNAL_OBJECT "/dvc/signal/Object"
#define DBUS_SIGNAL_SHOW "Show"

using callback_t = void (*)(void *);
using callback_object_t = void *;
using ms = std::chrono::milliseconds;

class DBusInterface {
public:
        DBusInterface() {}
        ~DBusInterface() noexcept
        {
                if(listener.joinable())
                {
                        shutdown = true;
                        listener.join();
                }
        }
        DBusInterface(const DBusInterface& src) = delete;
        DBusInterface& operator=(const DBusInterface& src) = delete;

        void spawnListener(callback_t cb, callback_object_t object) noexcept;
        void receive() noexcept;
        static void sendSignal(char *message) noexcept;

private:
        std::thread listener;
        callback_t callback_fn{nullptr};
        callback_object_t callback_object{nullptr};
        bool shutdown{false};
        ms sleep_ms{100};
};
#endif        // DBUS_HPP
