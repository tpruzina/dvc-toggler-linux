#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QSystemTrayIcon>
#include <QDialog>
#include <QCheckBox>

#include "config.hpp"
#include "profile_selector.hpp"
#include "procwatch.hpp"

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
	Config cfg;
	ProcWatch pw;
	friend class ProfileSelectorWidget;

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
};

#endif
