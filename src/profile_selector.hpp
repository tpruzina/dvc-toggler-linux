#ifndef PROFILESELECTORWIDGET_HPP
#define PROFILESELECTORWIDGET_HPP

//#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <QLineEdit>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>

#include "window.hpp"
#include "config.hpp"
#include "nvidia/nvidia.hpp"

class ProfileSelectorWidget : public QGroupBox
{
	Q_OBJECT
public:
	ProfileSelectorWidget(mainWindow *W, Config &cfg);

	QMap<int,int> dvc_map;
signals:

public slots:
	void apply_dvc();
	void updateComboBox();

private slots:
	void new_profile_clicked();
	void del_curr_profile_clicked();

private:
	void	createProfileTabsBox();
	void	createProfileSelectorButtonBox();

	QVBoxLayout profileSelectorWidgetLayout;
	QHBoxLayout profileSelectorLayout;
	QComboBox profileList;

	QGroupBox	profileSelectorBox;
	QPushButton	addProfileButton;
	QPushButton	delCurrentProfileButton;

	QTabWidget tabs;
	QVector<QString> profiles;
	QString	active_profile;

	mainWindow *W;
	Config &cfg;
	NVIDIA nv;

	friend class AppProfile;
	friend class DVCEntry;
};

class AppProfile : public QGroupBox
{
	Q_OBJECT
public:
	AppProfile(Config &cfg,  NVIDIA &nv, const QString name, ProfileSelectorWidget *p);


	ProfileSelectorWidget *PSW;
	const QString name;

signals:
public slots:

private:
	QVBoxLayout vlayout;
	Config &cfg;
	friend class DVCEntry;
};

class DVCEntry : public QGroupBox
{
	Q_OBJECT
public:
	explicit DVCEntry(const int dpyId, int dvc_level, AppProfile *p);

public slots:
	void onDVCSliderChanged(int value);

private:
	const int dpyId;
	int dvc;
	QHBoxLayout hlayout;
	QSlider dvc_slider;
	QLineEdit dvc_le;
	QLabel dpy_name;
	AppProfile *AP;
};

#endif // PROFILESELECTORWIDGET_HPP
