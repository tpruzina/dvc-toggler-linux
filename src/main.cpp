#include <QMessageBox>

#include "main_window.hpp"
#include "run_guard.hpp"
#include "dbus/dbus_watch.hpp"
#include "nvidia/nvidia.hpp"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(dvc_toggler_linux);
	QApplication app(argc, argv);

	// check whether we have QT system tray
	// FIXME: This doesn't actually exist on gnome3 and alikes ???
	if (!QSystemTrayIcon::isSystemTrayAvailable())
	{
		QMessageBox::critical(0, QObject::tr("DVC toggler"),
				      QObject::tr("I couldn't detect any system tray "
						  "on this system."));
		return 1;
	}

	// check whether DVC enabled nvidia screen is available
	if(!NVIDIA::isScreenAvailable())
	{
		QMessageBox::critical(0, QObject::tr("DVC toggler"),
				      QObject::tr("I couldn't detect any DVC enabled Nvidia screen."));
		return 1;
	}

	// check whether we are already running
	RunGuard guard("dvc_toggler_linux");
	if (!guard.tryToRun())
	{
		// signal mainWindow to show up (if hidden/minimized)
		DBusInterface bus;
		bus.sendSignal((char*)"show()");
		return 0;
	}

	QApplication::setQuitOnLastWindowClosed(false);

	// init and go
	mainWindow window;
	return app.exec();
}
