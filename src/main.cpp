#include <QMessageBox>

#include "main_window.hpp"
#include "runguard.hpp"

#include "dbus/dbus_watch.hpp"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(dvc_toggler_linux);

	RunGuard guard("dvc_toggler_linux");
	if (!guard.tryToRun())
	{
		DBusInterface bus;
		bus.sendsignal((char*)"show()");
		return 0;
	}

	QApplication app(argc, argv);

	if (!QSystemTrayIcon::isSystemTrayAvailable())
	{
		QMessageBox::critical(0, QObject::tr("DVC toggler"),
				      QObject::tr("I couldn't detect any system tray "
						  "on this system."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	mainWindow window;
	return app.exec();
}
