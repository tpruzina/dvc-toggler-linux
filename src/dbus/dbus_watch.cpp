#include <dbus/dbus.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "dbus_watch.hpp"

void
DBusInterface::sendsignal(char *message)
{
	DBusMessage *msg;
	DBusMessageIter args;
	DBusConnection *conn;
	dbus_uint32_t serial = 0;

	conn = dbus_bus_get(DBUS_BUS_STARTER, NULL);
	if (!conn)
		return;

	dbus_bus_request_name(conn, DVC_DBUS_CLIENT_NAME,
			      DBUS_NAME_FLAG_REPLACE_EXISTING, NULL);
	msg = dbus_message_new_signal(DVC_DBUS_SIGNAL_OBJECT, // object name of the signal
				      DVC_DBUS_IF_NAME,	// interface name of the signal
				      DVC_DBUS_SIGNAL_SHOW);	// name of the signal
	dbus_message_iter_init_append(msg, &args);
	dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &message);

	// send the message and flush the connection
	dbus_connection_send(conn, msg, &serial);
	dbus_connection_flush(conn);
	dbus_message_unref(msg);
	dbus_connection_close(conn);
}

void
DBusInterface::spawn_listener(void (*cb)(void*), void* foreign_object)
{
	callback_fn = cb;
	callback_object = foreign_object;
	listener = std::thread(&DBusInterface::receive, this);
}

void
DBusInterface::receive()
{
	DBusMessage *msg;
	DBusMessageIter args;
	DBusConnection *conn;

	if (!callback_fn)
		return;

	conn = dbus_bus_get(DBUS_BUS_STARTER, NULL);
	if (!conn)
		return;


	dbus_bus_request_name(conn, DVC_DBUS_HOST_SERVER,
			      DBUS_NAME_FLAG_REPLACE_EXISTING, NULL);

	dbus_bus_add_match(conn, "type='signal',interface='" DVC_DBUS_IF_NAME "'", NULL);	// see signals from the given interface
	dbus_connection_flush(conn);


	// loop listening for signals being emmitted
	while (true)
	{
		// non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);


		// loop again if we haven't read a message
		if (!msg) {
			std::this_thread::sleep_for(std::
						    chrono::milliseconds
						    (sleep_ms));
			continue;
		}
		// check if the message is a signal from the correct interface and with the correct name
		if (dbus_message_is_signal
		    (msg, DVC_DBUS_IF_NAME, DVC_DBUS_SIGNAL_SHOW))
		{
			std::cerr << "PARSING MESSAGE\n";
			// read the parameters
			dbus_message_iter_init(msg, &args);
			if (DBUS_TYPE_STRING ==
			    dbus_message_iter_get_arg_type(&args))
			{
				char *sigvalue = NULL;
				dbus_message_iter_get_basic(&args, &sigvalue);
				if (!strcmp(sigvalue, "show()"))
				{
					// display mainWindow
					callback_fn(callback_object);
				}

				std::cerr << sigvalue << std::endl;
			}
		}
		// free the message
		dbus_message_unref(msg);
	}
	// close the connection
	dbus_connection_close(conn);
}
