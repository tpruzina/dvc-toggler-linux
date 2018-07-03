#include "window.hpp"
#include "profile_selector.hpp"

mainWindow::mainWindow() :
	pw(nv)
{
	setWindowTitle(tr("DVC toggler"));

	createIconGroupBox();
	createTrayIcon();

	connect(enabledCheckBox, SIGNAL(clicked()), this, SLOT(toggleEnabled()));
	connect(closeToTrayCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleAutoHide()));
	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	QGroupBox *profileSelector = new ProfileSelectorWidget(this);
	profileSelector->setTitle("Profiles");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(iconGroupBox);
	mainLayout->addWidget(profileSelector);
	setLayout(mainLayout);

	trayIcon->show();
}

void mainWindow::setVisible(bool visible)
{
	minimizeAction->setEnabled(visible);
	restoreAction->setEnabled(!visible);
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
	if (trayIcon->isVisible())
	{
		trayIcon->showMessage("DVC toggler",
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
		pw.set_enabled(true);
	}

	trayIcon->setIcon(icon);
	trayIcon->setToolTip(status);
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
	trayIcon->showMessage("DVC toggler", msg, icon, timeout);
}

void mainWindow::messageClicked()
{
	QMessageBox::information(0, tr("Systray"),
				 tr("Sorry, I already gave what help I could.\n"
				    "Maybe you should try asking a human?"));
}

void mainWindow::createIconGroupBox()
{
	iconGroupBox = new QGroupBox(tr("Settings"));

	enabledCheckBox = new QCheckBox(tr("Enable"));
	bool enabled = cfg.query_enabled();
	enabledCheckBox->setChecked(enabled);
	pw.set_enabled(enabled);

	closeToTrayCheckBox = new QCheckBox(tr("Close to tray"));
	closeToTrayCheckBox->setChecked(cfg.query_autohide());

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(enabledCheckBox);
	iconLayout->addWidget(closeToTrayCheckBox);
	iconGroupBox->setLayout(iconLayout);
}

void mainWindow::createTrayIcon()
{
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	icon = (cfg.query_enabled()) ?
				QIcon(":/resources/enabled.png") :
				QIcon(":/resources/disabled.png");

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(icon);
	trayIcon->setContextMenu(trayIconMenu);
}
