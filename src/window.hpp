#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QMenu>
#include <QSystemTrayIcon>

#include "config.hpp"
#include "proc_watch.hpp"

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
	void messageClicked();
	void toggleAutoHide();

private:
	Config cfg;	// Persistent configuration object
	NVIDIA nv;	// DVC related nvidia object
	ProcWatch pw;	// Window watcher

	void createIconGroupBox();
	void createTrayIcon();

	QGroupBox *iconGroupBox;
	QCheckBox *closeToTrayCheckBox;
	QCheckBox *enabledCheckBox;

	QAction *minimizeAction;
	QAction *restoreAction;
	QAction *quitAction;

	QSystemTrayIcon *trayIcon;
	QIcon icon;
	QMenu *trayIconMenu;

	friend class ProfileSelectorWidget;
};
#endif
