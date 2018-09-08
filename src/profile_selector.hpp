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
	ProfileSelectorWidget(ProcWatch &pw, Config &cfg, NVIDIA &nv);

signals:

public slots:
	void applyDVC();
	void updateComboBox(int index);
	void removeProfile(int index);

private slots:


private:
	void	createProfileTabsBox();
	void	createProfileSelectorButtonBox();

	QVBoxLayout profileSelectorWidgetLayout;
	QHBoxLayout profileSelectorLayout;
	QComboBox profileList;

	QGroupBox	profileSelectorBox;

	QTabWidget tabs;

	ProcWatch &pw;
	Config &cfg;
	NVIDIA &nv;

	QMap<int,int> dvc_map;

        friend class AppProfile;
	friend class DVCEntry;
};

class AppProfile : public QGroupBox
{
	Q_OBJECT
public:
	AppProfile(const QString name, ProfileSelectorWidget *p);


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
	explicit DVCEntry(const int dpy_id, int dvc_level, AppProfile *p);

public slots:
	void onDVCSliderChanged(int value);

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
