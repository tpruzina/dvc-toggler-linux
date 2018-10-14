#include "main_window.hpp"
#include "profile_selector.hpp"
#include "desktop_entry.hpp"

mainWindow::mainWindow() noexcept :
        pw(
		nv,
		cfg.get_bool(CONFIG_ENABLED_STR),
		cfg.querySleepTime_ms()
	)
{
        setWindowTitle(tr("DVC toggler"));

        // set up dbus listener for "show()" messages
        bus.spawnListener(      [] (void *object)
                                {
                                        mainWindow *mW = static_cast<mainWindow*>(object);
                                        if(!mW->isVisible())
                                                mW->show();
                                },
                                this
        );

        // setup UI
        createSettingsBox();
        createTrayIcon();

        ProfileSelectorWidget *profileSelector = new ProfileSelectorWidget(pw, cfg, nv);
        profileSelector->setTitle("Profiles");

        mainLayout.addWidget(&settingsGroupBox);
        mainLayout.addWidget(profileSelector);
        setLayout(&mainLayout);

        setGeometry(0,0,300,250); // fixme remember size from last run

        if(cfg.get_bool(CONFIG_START_MIN_STR) == false)
                show();
}

auto mainWindow::createSettingsBox() noexcept -> void
{
        settingsGroupBox.setTitle(tr("Settings"));

        // setup checkboxes
        enabledCheckBox.setText(tr("Enable"));
        bool enabled_state = cfg.get_bool(CONFIG_ENABLED_STR);
        enabledCheckBox.setChecked(enabled_state);
        pw.setEnabled(enabled_state);

        startHiddenCheckBox.setText(tr("Start minimized"));

        startHiddenCheckBox.setChecked(cfg.get_bool(CONFIG_START_MIN_STR));

        autostartCheckBox.setText(tr("Autostart"));
        autostartCheckBox.setChecked(DesktopEntry::exists(
                                             DesktopEntry::getAutostartPath()));
//      autostartCheckBox.setDisabled(true);

        if (QSystemTrayIcon::isSystemTrayAvailable())
                autohideCheckBox.setText(tr("Close to tray"));
        else
                autohideCheckBox.setText(tr("Hide on exit"));

        autohideCheckBox.setChecked(cfg.get_bool(CONFIG_AUTOHIDE_STR));

        QGridLayout *iconLayout = new QGridLayout;
        iconLayout->addWidget(&enabledCheckBox, 0, 0);
        iconLayout->addWidget(&startHiddenCheckBox, 0, 1);
        iconLayout->addWidget(&autostartCheckBox, 1, 0);
        iconLayout->addWidget(&autohideCheckBox, 1, 1);
        settingsGroupBox.setLayout(iconLayout);

        connect(&startHiddenCheckBox, SIGNAL(clicked()), this, SLOT(toggleStartHidden()));
        connect(&autostartCheckBox, SIGNAL(clicked()), this, SLOT(toggleAutoStart()));
        connect(&enabledCheckBox, SIGNAL(clicked()), this, SLOT(toggleEnabled()));
        connect(&autohideCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleAutoHide()));
}

auto mainWindow::createTrayIcon() noexcept -> void
{
        // don't setup tray icon if system doesn't support it
        if (!QSystemTrayIcon::isSystemTrayAvailable())
                return;

        minimizeAction.setText(tr("Mi&nimize"));
        connect(&minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

        restoreAction.setText(tr("&Restore"));
        connect(&restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

        quitAction.setText(tr("&Quit"));
        connect(&quitAction, SIGNAL(triggered()), this, SLOT(quit()));

        trayIconMenu.addAction(&minimizeAction);
        trayIconMenu.addAction(&restoreAction);
        trayIconMenu.addSeparator();
        trayIconMenu.addAction(&quitAction);

        icon = (cfg.get_bool(CONFIG_ENABLED_STR)) ?
                                QIcon(":/resources/enabled.png") :
                                QIcon(":/resources/disabled.png");

        trayIcon.setIcon(icon);
        trayIcon.setContextMenu(&trayIconMenu);

        connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

        if(cfg.get_bool(CONFIG_START_MIN_STR))
                minimizeAction.setEnabled(false);

        trayIcon.show();
}

auto mainWindow::setVisible(bool visible) noexcept -> void
{
        minimizeAction.setEnabled(visible);
        restoreAction.setEnabled(!visible);
        QDialog::setVisible(visible);
}

auto mainWindow::closeEvent(QCloseEvent *event) noexcept -> void
{
        if(cfg.get_bool(CONFIG_AUTOHIDE_STR) == false)
                this->quit();

        if (trayIcon.isVisible())
        {
                // show bubble pointing user to tray icon (only once)
                if(cfg.get_bool(CONFIG_TRAY_INFO_SHOWN) == false)
                {
                        trayIcon.showMessage("DVC toggler",
                                     tr("The program will keep running in the "
                                        "system tray.\nTo terminate the program, "
                                        "choose \"Quit\" in the context menu "
                                        "of the system tray entry."),
                                     icon,
                                     5000);
                        cfg.set_bool(CONFIG_TRAY_INFO_SHOWN, true);
                }
                hide();
                event->ignore();
        }
}

auto mainWindow::toggleStartHidden() noexcept -> void
{
        cfg.toggle_bool(CONFIG_START_MIN_STR);
}

auto mainWindow::toggleAutoStart() noexcept -> void
{
        QString as_file_path = DesktopEntry::getAutostartPath();

        if(!DesktopEntry::exists(as_file_path))
                DesktopEntry::create(as_file_path);
        else
                DesktopEntry::remove(as_file_path);
}

auto mainWindow::toggleEnabled() noexcept -> void
{

        QString status;
        if(cfg.toggle_bool(CONFIG_ENABLED_STR))
        {
                icon = QIcon(":/resources/enabled.png");
                status = "DVC enabled";
                pw.setEnabled(true);
        }
        else
        {
                status = "DVC disabled";
                icon = QIcon(":/resources/disabled.png");
                pw.setEnabled(false);
        }

        trayIcon.setIcon(icon);
        trayIcon.setToolTip(status);
        setWindowIcon(icon);
}

auto mainWindow::toggleAutoHide() noexcept -> void
{
        cfg.toggle_bool(CONFIG_AUTOHIDE_STR);
}

auto mainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) noexcept -> void
{
        switch (reason)
        {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
                toggleEnabled();
                break;
        case QSystemTrayIcon::MiddleClick:
                if(isVisible())
                        hide();
                else
                        show();
                break;
        default:
                ;
        }
}

auto mainWindow::showMessage(const QString &msg, unsigned timeout) noexcept -> void
{
        trayIcon.showMessage("DVC toggler", msg, icon, timeout);
}

auto mainWindow::quit() noexcept -> void
{
        cfg.sync();
        exit(0);
}
