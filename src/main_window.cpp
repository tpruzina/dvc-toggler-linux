#include "main_window.hpp"
#include "profile_selector.hpp"

mainWindow::mainWindow() :
	pw(nv, cfg.queryEnabled(), cfg.querySleepTime_ms())

{
	setWindowTitle(tr("DVC toggler"));

	// fail with MessageBox if nvidia enabled screen isn't available
	if(!nv.isScreenAvailable())
	{
		QMessageBox::critical(0, QObject::tr("DVC toggler"),
				      QObject::tr("Unable to find any NVIDIA X screens"));
		exit(1);
	}

	// set up dbus listener for "show()" messages
	bus.spawnListener(	[] (void *object)
				{
					mainWindow *mW = (mainWindow *) object;
					if(!mW->isVisible())
						mW->show();
				},
				this
	);

	// setup ID
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

	setGeometry(0,0,300,250); // fixme remember size from last run

	trayIcon.show();
	this->show();
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
mainWindow::createIconGroupBox()
{
	iconGroupBox.setTitle(tr("Settings"));

	enabledCheckBox.setText(tr("Enable"));
	bool enabled = cfg.queryEnabled();
	enabledCheckBox.setChecked(enabled);
	pw.setEnabled(enabled);

	closeToTrayCheckBox.setText(tr("Close to tray"));
	closeToTrayCheckBox.setChecked(cfg.queryAutohide());

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(&enabledCheckBox);
	iconLayout->addWidget(&closeToTrayCheckBox);
	iconGroupBox.setLayout(iconLayout);
}

void
mainWindow::quit()
{
	cfg.sync();
	exit(0);
}

void
mainWindow::createTrayIcon()
{
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
}
