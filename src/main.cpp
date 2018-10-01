#include <QMessageBox>

#include "main_window.hpp"
#include "run_guard/run_guard.hpp"
#include "dbus/dbus_watch.hpp"
#include "nvidia/nvidia.hpp"

auto main(int argc, char *argv[]) noexcept -> int
{
        Q_INIT_RESOURCE(dvc_toggler_linux);
        QApplication app(argc, argv);

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
