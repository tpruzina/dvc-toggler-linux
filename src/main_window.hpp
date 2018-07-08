#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QGridLayout>

#include "config.hpp"
#include "proc_watch.hpp"
#include "dbus/dbus_watch.hpp"


class mainWindow : public QDialog
{
	Q_OBJECT

public:
	mainWindow();

	void setVisible(bool visible);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void toggleEnabled();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString &msg, unsigned timeout = 3000);
	void toggleAutoHide();
	void quit();

private:
	Config cfg;	// Persistent configuration object
	NVIDIA nv;	// DVC related nvidia object
	ProcWatch pw;	// Window watcher
	DBusInterface bus;


	void createSettingsBox();
	void createTrayIcon();

	QGroupBox settingsGroupBox;

	QCheckBox autohideCheckBox;
	QCheckBox enabledCheckBox;
	QCheckBox startHiddenCheckBox;
	QCheckBox autostartCheckBox;

	QVBoxLayout mainLayout;

	QAction minimizeAction;
	QAction restoreAction;
	QAction quitAction;

	QSystemTrayIcon trayIcon;
	QIcon icon;
	QMenu trayIconMenu;

	friend class ProfileSelectorWidget;
	friend class AppProfile;
	friend class DVCEntry;
};
#endif // MAIN_WINDOW_HPP
