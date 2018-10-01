#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtWidgets>

#include "config.hpp"
#include "proc_watch.hpp"
#include "dbus/dbus_watch.hpp"


class mainWindow : public QDialog
{
        Q_OBJECT

public:
        mainWindow() noexcept;
        auto setVisible(bool visible) noexcept -> void;

protected:
        auto closeEvent(QCloseEvent *event) noexcept -> void;

private slots:
        void toggleEnabled() noexcept;
        void toggleStartHidden() noexcept;
        void toggleAutoStart() noexcept;
        void toggleAutoHide() noexcept;

        void iconActivated(QSystemTrayIcon::ActivationReason reason) noexcept;
        void showMessage(const QString &msg, unsigned timeout = 3000) noexcept;
        void quit() noexcept;

private:
        Config cfg;        // Persistent configuration object
        NVIDIA nv;        // DVC related nvidia object
        ProcWatch pw;        // Window watcher
        DBusInterface bus;


        void createSettingsBox() noexcept;
        void createTrayIcon() noexcept;

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
