#include <QtWidgets>

#include "window.hpp"
#include "runguard.hpp"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(dvc_toggler_linux);

	RunGuard guard("dvc_toggler_linux");
	if (!guard.tryToRun())
	{
		// TODO: send signal to main application to ->show()
		// instead of failing silently if toggler is already running
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

	Window window;
	window.show();
	return app.exec();
}
