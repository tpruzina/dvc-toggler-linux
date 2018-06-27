#include <QtWidgets>

#include "window.hpp"
#include "profile_selector.hpp"

Window::Window()
{
	setWindowTitle(tr("DVC toggler"));

	createIconGroupBox();
	createMessageGroupBox();

	createTrayIcon();

	connect(enabledCheckBox, SIGNAL(clicked()), this, SLOT(toggleEnabled()));
	connect(closeToTrayCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleAutoHide()));
	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	QGroupBox *profileSelector = new ProfileSelectorWidget(cfg);
	profileSelector->setTitle("Profiles");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(iconGroupBox);
	mainLayout->addWidget(profileSelector);
	setLayout(mainLayout);

	trayIcon->show();
}

void Window::setVisible(bool visible)
{
	minimizeAction->setEnabled(visible);
	maximizeAction->setEnabled(!isMaximized());
	restoreAction->setEnabled(isMaximized() || !visible);
	QDialog::setVisible(visible);
	qDebug() << "setVisibile(" << visible << ") called!";
}

void Window::closeEvent(QCloseEvent *event)
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

void Window::toggleEnabled()
{
	cfg.toggle_enabled();
	QString status;
	if(cfg.query_enabled())
	{
		icon = QIcon(":/images/enabled.png");
		status = "DVC enabled";
	}
	else
	{
		status = "DVC disabled";
		icon = QIcon(":/images/disabled.png");
	}

	trayIcon->setIcon(icon);
	trayIcon->setToolTip(status);
	setWindowIcon(icon);
}

void Window::toggleAutoHide()
{
	cfg.toggle_autohide();
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

void Window::showMessage(const QString &msg, unsigned timeout)
{
	trayIcon->showMessage("DVC toggler", msg, icon, timeout);
}

void Window::messageClicked()
{
	QMessageBox::information(0, tr("Systray"),
				 tr("Sorry, I already gave what help I could.\n"
				    "Maybe you should try asking a human?"));
}

void Window::createIconGroupBox()
{
	iconGroupBox = new QGroupBox(tr("Settings"));

	enabledCheckBox = new QCheckBox(tr("Enable"));
	enabledCheckBox->setChecked(cfg.query_enabled());

	closeToTrayCheckBox = new QCheckBox(tr("Close to tray"));
	closeToTrayCheckBox->setChecked(cfg.query_autohide());

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(enabledCheckBox);
	iconLayout->addWidget(closeToTrayCheckBox);
	iconGroupBox->setLayout(iconLayout);
}

void Window::createMessageGroupBox()
{

}

void Window::createTrayIcon()
{
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	maximizeAction = new QAction(tr("Ma&ximize"), this);
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	icon = (cfg.query_enabled()) ?
				QIcon(":/images/enabled.png") :
				QIcon(":/images/disabled.png");

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(icon);
	trayIcon->setContextMenu(trayIconMenu);
}
