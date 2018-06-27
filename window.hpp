#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QDialog>

#include "config.hpp"
#include "profile_selector.hpp"

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;

class Window : public QDialog
{
	Q_OBJECT

public:
	Window();

	void setVisible(bool visible);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void toggleEnabled();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString &msg, unsigned timeout = 3000);
	void messageClicked();

private:
	Config cfg;
	friend class ProfileSelectorWidget;

	void createIconGroupBox();
	void createMessageGroupBox();
	void createTrayIcon();

	QGroupBox *iconGroupBox;
	QCheckBox *autohideCheckBox;
	QCheckBox *enabledCheckBox;

	QAction *minimizeAction;
	QAction *maximizeAction;
	QAction *restoreAction;
	QAction *quitAction;

	QSystemTrayIcon *trayIcon;
	QIcon icon;
	QMenu *trayIconMenu;
};

#endif
