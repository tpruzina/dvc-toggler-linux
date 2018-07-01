#include "profile_selector.hpp"

ProfileSelectorWidget::ProfileSelectorWidget(Config &cfg) :
	cfg(cfg)

{
	// build profile tabs from Config
	for(const auto &app : cfg.query_profiles())
	{
		QIcon icon(cfg.query_icon_path(app));
		// todo fix leak of AppProfile(s)
		tabs.addTab(new AppProfile(cfg, app), icon, app);
		profiles.push_back(app);
		active_profile = app;
		qDebug() << "iconpath for" << app << "is " << cfg.query_icon_path(app);
	}
	tabs.setTabsClosable(true);


	// build profile selector buttons area
	createProfileSelectorButtonBox();


	QVBoxLayout *profileSelectorWidgetLayout = new QVBoxLayout;
	profileSelectorWidgetLayout->addWidget(&tabs);
	profileSelectorWidgetLayout->addWidget(profileSelectorBox);
	this->setLayout(profileSelectorWidgetLayout);
}

void
ProfileSelectorWidget::createProfileSelectorButtonBox()
{
	profileSelectorBox = new QGroupBox();
	profileSelectorBox->setStyleSheet("border:none");

	QComboBox *profileList = new QComboBox;

	QStringList list=(QStringList()<<"red"<<"yellow"<<"blue");
	profileList->addItems(list);

	addProfileButton = new QPushButton;
	addProfileButton->setIcon(QIcon(":/resources/plus.svg"));
	addProfileButton->setMaximumWidth(30);
	connect(addProfileButton, SIGNAL(clicked()), this, SLOT(new_profile_clicked()));

	delCurrentProfileButton = new QPushButton;
	delCurrentProfileButton->setIcon(QIcon(":/resources/minus.svg"));
	delCurrentProfileButton->setMaximumWidth(30);
	connect(delCurrentProfileButton, SIGNAL(clicked()), this, SLOT(del_curr_profile_clicked()));

	QHBoxLayout *profileSelectorLayout = new QHBoxLayout;
	profileSelectorLayout->addWidget(profileList);
	profileSelectorLayout->addWidget(addProfileButton);
	profileSelectorLayout->addWidget(delCurrentProfileButton);
	profileSelectorBox->setLayout(profileSelectorLayout);
}

void
ProfileSelectorWidget::new_profile_clicked()
{
	QMessageBox::information( this, "Clicked!", "The button was clicked!" );
}

void
ProfileSelectorWidget::del_curr_profile_clicked()
{
	QMessageBox::information( this, "Clicked!", "The button was clicked!" );
}

AppProfile::AppProfile(Config &cfg, const QString &name) :
	_cfg(cfg),
	_name(name)
{

	 QGroupBox *dvcgb = new QGroupBox;
	QHBoxLayout *hlayout = new QHBoxLayout;

	dvc_slider = new QSlider(Qt::Horizontal);
	dvc_slider->setMinimum(-100);
	dvc_slider->setMaximum(100);
	dvc_slider->setValue(dvc);

	dvc_le = new QLineEdit();
	dvc_le->setText(QString::number(dvc));
	dvc_le->setReadOnly(true);

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
	this->setStyleSheet("border:none");
}

void
AppProfile::onDVCSliderChanged(int value)
{
	dvc_le->setText(QString::number(dvc_slider->sliderPosition()));
	dvc = value;
	_cfg.set_dvc(_name, value);
}
