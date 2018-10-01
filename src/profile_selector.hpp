#ifndef PROFILESELECTORWIDGET_HPP
#define PROFILESELECTORWIDGET_HPP

#include <QtWidgets>

#include "config.hpp"
#include "nvidia/nvidia.hpp"
#include "proc_watch.hpp"

class ProfileSelectorWidget : public QGroupBox
{
        Q_OBJECT
public:
        ProfileSelectorWidget(ProcWatch &pw, Config &cfg, NVIDIA &nv) noexcept;

signals:

public slots:
        void applyDVC() noexcept;
        void updateComboBox(int index) noexcept;
        void removeProfile(int index) noexcept;

private slots:


private:
        void        createProfileTabsBox() noexcept;
        void        createProfileSelectorButtonBox() noexcept;

        QVBoxLayout profileSelectorWidgetLayout;
        QHBoxLayout profileSelectorLayout;
        QComboBox profileList;

        QGroupBox        profileSelectorBox;

        QTabWidget tabs;

        ProcWatch &pw;
        Config &cfg;
        NVIDIA &nv;

        DVC_map dvc_map;

        friend class AppProfile;
        friend class DVCEntry;
};

class AppProfile : public QGroupBox
{
        Q_OBJECT
public:
        AppProfile(const QString name, ProfileSelectorWidget *p) noexcept;


        ProfileSelectorWidget *PSW;
        const QString name;

private:
        QVBoxLayout vlayout;
        friend class DVCEntry;
};

class DVCEntry : public QGroupBox
{
        Q_OBJECT
public:
        explicit DVCEntry(const int dpy_id, int dvc_level, AppProfile *p) noexcept;

public slots:
        void onDVCSliderChanged(int value) noexcept;

private:
        const int dpy_id;
        int dvc;
        QHBoxLayout hlayout;
        QSlider dvc_slider;
        QLineEdit dvc_le;
        QLabel dpy_name;
        AppProfile *AP;
};

#endif // PROFILESELECTORWIDGET_HPP
