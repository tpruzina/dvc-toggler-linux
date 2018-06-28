#include "profile_selector.hpp"

ProfileSelectorWidget::ProfileSelectorWidget(Config &cfg) :
	cfg(cfg)

{
	QTabWidget *tabs = new QTabWidget;
	for(const auto &app : cfg.query_profiles())
	{
		QIcon icon(cfg.query_icon_path(app));
		tabs->addTab(new AppProfile(cfg, app), icon, app);
		qDebug() << "iconpath for" << app << "is " << cfg.query_icon_path(app);
	}

	QVBoxLayout *profileSelectorWidgetLayout = new QVBoxLayout;
	profileSelectorWidgetLayout->addWidget(tabs);
	this->setLayout(profileSelectorWidgetLayout);
}

AppProfile::AppProfile(Config &cfg, const QString &name) :
	_cfg(cfg),
	_name(name),
	dvc(cfg.query_dvc(name))
{
	QGroupBox *dvcgb = new QGroupBox;
	QHBoxLayout *hlayout = new QHBoxLayout;

	dvc_slider = new QSlider(Qt::Horizontal);
	dvc_slider->setMinimum(-1024);
	dvc_slider->setMaximum(1024);
	dvc_slider->setValue(dvc);

	dvc_le = new QLineEdit();
	dvc_le->setText(QString::number(dvc));

	connect(dvc_slider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(onDVCSliderChanged(int))
	);

	hlayout->addWidget(dvc_slider);
	hlayout->addWidget(dvc_le);
	dvcgb->setLayout(hlayout);

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addWidget(dvcgb);
	this->setLayout(vlayout);
}

void
AppProfile::onDVCSliderChanged(int value)
{
	dvc_le->setText(QString::number(dvc_slider->sliderPosition()));
	dvc = value;
	_cfg.set_dvc(_name, value);
}
