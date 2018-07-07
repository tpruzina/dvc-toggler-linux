#include "main_window.hpp"
#include "profile_selector.hpp"

mainWindow::mainWindow() :
	pw(nv, cfg.query_enabled(), cfg.query_sleep_time_ms())

{
	setWindowTitle(tr("DVC toggler"));

	if(!nv.check_available_screen())
	{
		QMessageBox::critical(0, QObject::tr("DVC toggler"),
			QObject::tr("Unable to find any NVIDIA X screens"));
		exit(1);
	}

	// set up dbus listener for "show()" messages
	bus.spawn_listener(	[] (void *object)
				{
					mainWindow *mW = (mainWindow *) object;
					if(!mW->isVisible())
						mW->show();
				}
				,this
	);

	createIconGroupBox();
	createTrayIcon();

	connect(&enabledCheckBox, SIGNAL(clicked()), this, SLOT(toggleEnabled()));
	connect(&closeToTrayCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleAutoHide()));
	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	ProfileSelectorWidget *profileSelector = new ProfileSelectorWidget(pw, cfg, nv);
	profileSelector->setTitle("Profiles");

	mainLayout.addWidget(&iconGroupBox);
	mainLayout.addWidget(profileSelector);
	setLayout(&mainLayout);

	setGeometry(0,0,300,250);

	trayIcon.show();
	this->show();
}

void mainWindow::setVisible(bool visible)
{
	minimizeAction.setEnabled(visible);
	restoreAction.setEnabled(!visible);
	QDialog::setVisible(visible);
	qDebug() << "setVisibile(" << visible << ") called!";
}

void mainWindow::closeEvent(QCloseEvent *event)
{
	if(cfg.query_autohide() == false)
	{
		cfg.sync();
		exit(0);
	}
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

void mainWindow::toggleEnabled()
{
	cfg.toggle_enabled();
	QString status;
	if(cfg.query_enabled())
	{
		icon = QIcon(":/resources/enabled.png");
		status = "DVC enabled";
		pw.set_enabled(true);
	}
	else
	{
		status = "DVC disabled";
		icon = QIcon(":/resources/disabled.png");
		pw.set_enabled(false);
	}

	trayIcon.setIcon(icon);
	trayIcon.setToolTip(status);
	setWindowIcon(icon);
}

void mainWindow::toggleAutoHide()
{
	cfg.toggle_autohide();
}

void mainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
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

void mainWindow::showMessage(const QString &msg, unsigned timeout)
{
	trayIcon.showMessage("DVC toggler", msg, icon, timeout);
}

void mainWindow::createIconGroupBox()
{
	iconGroupBox.setTitle(tr("Settings"));

	enabledCheckBox.setText(tr("Enable"));
	bool enabled = cfg.query_enabled();
	enabledCheckBox.setChecked(enabled);
	pw.set_enabled(enabled);

	closeToTrayCheckBox.setText(tr("Close to tray"));
	closeToTrayCheckBox.setChecked(cfg.query_autohide());

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(&enabledCheckBox);
	iconLayout->addWidget(&closeToTrayCheckBox);
	iconGroupBox.setLayout(iconLayout);
}

void mainWindow::createTrayIcon()
{
	minimizeAction.setText(tr("Mi&nimize"));
	connect(&minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	restoreAction.setText(tr("&Restore"));
	connect(&restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction.setText(tr("&Quit"));
	connect(&quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu.addAction(&minimizeAction);
	trayIconMenu.addAction(&restoreAction);
	trayIconMenu.addSeparator();
	trayIconMenu.addAction(&quitAction);

	icon = (cfg.query_enabled()) ?
				QIcon(":/resources/enabled.png") :
				QIcon(":/resources/disabled.png");

	trayIcon.setIcon(icon);
	trayIcon.setContextMenu(&trayIconMenu);
}
