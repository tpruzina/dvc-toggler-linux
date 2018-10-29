#include "dbus_watch.hpp"

#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals; // <num literal>ms
using std::this_thread::sleep_for;

auto listener() -> int
{
        auto received = false;
        {
                std::cout << "creating dbus listener\n";
                auto dbus = DBusInterface();
                dbus.spawnListener(
                        [] (callback_object_t object)
                        {
                                std::cout << "dbus listener: received\n";
                                bool *received = static_cast<bool*>(object);
                                *received = true;
                        },
                        &received
                );
                std::cout << "listener: waiting\n";
                sleep_for(500ms);
                std::cout << "listener: exiting\n";
        } // calls ~DBusInterface()
        std::cout << "listener: exited\n";
        return received ? 0 : -1;
}

auto sender() -> int
{
        sleep_for(100ms);
        auto dbus = DBusInterface();
        std::cout << "dbus client: sending message to listener\n";
        dbus.sendSignal((char*)"show()");
        return 0;
}

auto main() noexcept -> int
{
        pid_t pid;

        if((pid = fork()) != 0)
                return listener();
        else 
                return sender();
}
