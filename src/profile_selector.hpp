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

#include "config.hpp"
#include "nvidia/nvidia.hpp"

class ProfileSelectorWidget : public QGroupBox
{
	Q_OBJECT
public:
	explicit ProfileSelectorWidget(Config &parent);

signals:

public slots:

private slots:
	void new_profile_clicked();
	void del_curr_profile_clicked();

private:
	QGroupBox	*profileSelectorBox;
	QPushButton	*addProfileButton;
	QPushButton	*delCurrentProfileButton;

	void	createProfileSelectorButtonBox();

	QTabWidget tabs;
	QVector<QString> profiles;
	QString	active_profile;
	const Config &cfg;
};

class AppProfile : public QGroupBox
{
	Q_OBJECT
public:
	explicit AppProfile(Config &cfg, const QString &name);
signals:
public slots:

private:
	Config &_cfg;
	QString _name;
};

class DVCEntry : public QGroupBox
{
	Q_OBJECT
public:
	explicit DVCEntry(int dpyId, int &dvc_level);

public slots:
	void onDVCSliderChanged(int value);

private:
	const int dpyId;
	int &dvc;
	QSlider *dvc_slider;
	QLineEdit *dvc_le;
};

#endif // PROFILESELECTORWIDGET_HPP
