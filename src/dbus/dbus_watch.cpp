#include "dbus_watch.hpp"

#include <cstdlib>
#include <cstring>
#include <dbus/dbus.h>
#include <iostream>
#include <unistd.h>

auto DBusInterface::sendSignal(char *message) noexcept -> void
{
        auto args = DBusMessageIter{};
        dbus_uint32_t serial = 0;

        auto conn = dbus_bus_get(DBUS_BUS_STARTER, nullptr);
        if (!conn)
                return;

        dbus_bus_request_name(
                conn,
                DBUS_CLIENT_NAME,
                DBUS_NAME_FLAG_REPLACE_EXISTING,
                nullptr);

        auto msg = dbus_message_new_signal(
                DBUS_SIGNAL_OBJECT,        // object name of the signal
                DBUS_IF_NAME,        // interface name of the signal
                DBUS_SIGNAL_SHOW        // name of the signal
        );

        dbus_message_iter_init_append(msg, &args);
        dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &message);

        // send the message and flush the connection
        dbus_connection_send(conn, msg, &serial);
        dbus_connection_flush(conn);
        dbus_message_unref(msg);
        dbus_bus_release_name(conn, DBUS_CLIENT_NAME, nullptr);
        dbus_connection_unref(conn);
        return;
}

auto DBusInterface::spawnListener(void (*cb)(void*), void* object) noexcept -> void
{
        this->callback_fn = cb;
        this->callback_object = object;
        listener = std::thread(&DBusInterface::receive, this);
        return;
}

auto DBusInterface::receive() noexcept -> void
{
        if (!callback_fn)
                return;

        auto conn = dbus_bus_get(DBUS_BUS_STARTER, nullptr);
        if (!conn)
                return;

        dbus_bus_request_name(
                        conn,
                        DBUS_HOST_SERVER,
                        DBUS_NAME_FLAG_REPLACE_EXISTING,
                        NULL
        );

        dbus_bus_add_match(
                        conn,
                        "type='signal',interface='" DBUS_IF_NAME "'",
                        nullptr);

        dbus_connection_flush(conn);

        // loop listening for signals being emmitted
        while (true)
        {
                if(shutdown)
                        break;

                // non blocking read of the next available message
                dbus_connection_read_write(conn, 0);
                auto msg = dbus_connection_pop_message(conn);

                // loop again if we haven't read a message
                if (!msg)
                {
                        std::this_thread::sleep_for(
                                std::chrono::milliseconds(sleep_ms));
                        continue;
                }

                // check if the message is a signal from the correct interface and with the correct name
                if (dbus_message_is_signal(
                                        msg,
                                        DBUS_IF_NAME,
                                        DBUS_SIGNAL_SHOW))
                {
                        auto args = DBusMessageIter{};
                        // read the parameters
                        dbus_message_iter_init(msg, &args);
                        if (DBUS_TYPE_STRING ==
                            dbus_message_iter_get_arg_type(&args))
                        {
                                auto sigvalue = (char const *)(nullptr);
                                dbus_message_iter_get_basic(&args, &sigvalue);

                                // display mainWindow
                                if (!strcmp(sigvalue, "show()"))
                                        callback_fn(callback_object);
                        }
                }
                // free the message
                dbus_message_unref(msg);
        }
        // close the connection
        dbus_bus_remove_match(
                        conn, 
                        "type='signal',interface='" DBUS_IF_NAME "'",
                        nullptr);
        dbus_bus_release_name(conn, DBUS_HOST_SERVER, nullptr);
        dbus_connection_unref(conn);
        return;
}

