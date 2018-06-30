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

#include "config.hpp"

class ProfileSelectorWidget : public QGroupBox
{
	Q_OBJECT
public:
	explicit ProfileSelectorWidget(Config &parent);

signals:

public slots:

private:
	const Config &cfg;
};

class AppProfile : public QGroupBox
{
	Q_OBJECT
public:
	explicit AppProfile(Config &cfg, const QString &name);
signals:

public slots:
	void onDVCSliderChanged(int value);

private:
	QSlider *dvc_slider;
	QLineEdit *dvc_le;
	Config &_cfg;
	QString _name;
	int dvc;
};

#endif // PROFILESELECTORWIDGET_HPP
