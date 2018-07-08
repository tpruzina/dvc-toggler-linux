#include "main_window.hpp"
#include "profile_selector.hpp"

mainWindow::mainWindow() :
	pw(nv, cfg.queryEnabled(), cfg.querySleepTime_ms())

{
	setWindowTitle(tr("DVC toggler"));

	// set up dbus listener for "show()" messages
	bus.spawnListener(	[] (void *object)
				{
					mainWindow *mW = (mainWindow *) object;
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

	this->show();
}

void
mainWindow::createSettingsBox()
{
	settingsGroupBox.setTitle(tr("Settings"));

	enabledCheckBox.setText(tr("Enable"));
	bool enabled = cfg.queryEnabled();
	enabledCheckBox.setChecked(enabled);
	pw.setEnabled(enabled);

	if (QSystemTrayIcon::isSystemTrayAvailable())
		autohideCheckBox.setText(tr("Close to tray"));
	else
		autohideCheckBox.setText(tr("Hide on exit"));

	autohideCheckBox.setChecked(cfg.queryAutohide());

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(&enabledCheckBox);
	iconLayout->addWidget(&autohideCheckBox);
	settingsGroupBox.setLayout(iconLayout);


	connect(&enabledCheckBox, SIGNAL(clicked()), this, SLOT(toggleEnabled()));
	connect(&autohideCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleAutoHide()));
}

void
mainWindow::createTrayIcon()
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

	icon = (cfg.queryEnabled()) ?
				QIcon(":/resources/enabled.png") :
				QIcon(":/resources/disabled.png");

	trayIcon.setIcon(icon);
	trayIcon.setContextMenu(&trayIconMenu);

	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	trayIcon.show();
}

void
mainWindow::setVisible(bool visible)
{
	minimizeAction.setEnabled(visible);
	restoreAction.setEnabled(!visible);
	QDialog::setVisible(visible);
	qDebug() << "setVisibile(" << visible << ") called!";
}

void
mainWindow::closeEvent(QCloseEvent *event)
{
	if(cfg.queryAutohide() == false)
		this->quit();

	if (trayIcon.isVisible())
	{
		trayIcon.showMessage("DVC toggler",
				     tr("The program will keep running in the "
					"system tray.\nTo terminate the program, "
					"choose \"Quit\" in the context menu "
					"of the system tray entry."),
				     icon,
				     3000);
		hide();
		event->ignore();
	}
}

void
mainWindow::toggleEnabled()
{
	cfg.toggleEnabled();
	QString status;
	if(cfg.queryEnabled())
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

void
mainWindow::toggleAutoHide()
{
	cfg.toggleAutohide();
}

void
mainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		toggleEnabled();
		break;
	case QSystemTrayIcon::MiddleClick:
		showMessage("test");
		break;
	default:
		;
	}
}

void
mainWindow::showMessage(const QString &msg, unsigned timeout)
{
	trayIcon.showMessage("DVC toggler", msg, icon, timeout);
}

void
mainWindow::quit()
{
	cfg.sync();
	exit(0);
}
