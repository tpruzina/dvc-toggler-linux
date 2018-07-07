#include <iostream>
#include "dbus.hpp"
#include <unistd.h> // C sleep

int
main(int argc, char **argv)
{
    DBusInterface *dbus = new DBusInterface;

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
