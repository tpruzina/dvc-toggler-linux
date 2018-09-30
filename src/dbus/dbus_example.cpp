#include <iostream>
#include "dbus.hpp"
#include <unistd.h> // C sleep

auto main(int argc, char **argv) noexcept -> int
{
        auto dbus = new DBusInterface;

        // TODO: fork, sleep test
        if(argc == 1)
        {
                dbus->spawn_listener(
                        [] (void *none)
                        {
                                std::cout << "Callback called" << std::endl;
                        },
                        NULL
                );
                sleep(100);
        }
        else
                dbus->sendsignal((char*)"show()");

        delete dbus;
        return 0;
}
