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
	ProfileSelectorWidget(mainWindow *W);

	QMap<int,int> dvc_map;
signals:

public slots:
	void apply_dvc();
	void updateComboBox(int line);
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

	mainWindow *W;
	Config &cfg;
	NVIDIA &nv;

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

signals:
public slots:

private:
	QVBoxLayout vlayout;
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
