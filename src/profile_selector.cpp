#include "profile_selector.hpp"

ProfileSelectorWidget::ProfileSelectorWidget(mainWindow *p, Config &cfg) :
	W(p),
	cfg(cfg)
{
	dvc_map = QMap<int,int>(nv.get_vibrance());
	// build profile tabs from Config
	createProfileTabsBox();

	// build profile selector buttons area
	createProfileSelectorButtonBox();


	profileSelectorWidgetLayout.addWidget(&tabs);
	profileSelectorWidgetLayout.addWidget(&profileSelectorBox);
	this->setLayout(&profileSelectorWidgetLayout);
}

void
ProfileSelectorWidget::createProfileTabsBox()
{
	for(const auto &app : cfg.query_profiles())
	{
		QIcon icon(cfg.query_icon_path(app));			
		tabs.addTab(new AppProfile(cfg, nv, app, this), icon, app);
		profiles.push_back(app);
		active_profile = app;
		qDebug() << "iconpath for" << app << "is " << cfg.query_icon_path(app);
	}
	//	tabs.setTabsClosable(true);
	//	tabs.tabBar()->tabButton(0,QTabBar::RightSide)->hide();
	tabs.setUsesScrollButtons(true);

}

void
ProfileSelectorWidget::createProfileSelectorButtonBox()
{
	profileSelectorBox.setStyleSheet("border:none");

	updateComboBox();

	addProfileButton.setIcon(QIcon(":/resources/plus.svg"));
	addProfileButton.setMaximumWidth(30);
	connect(&addProfileButton, SIGNAL(clicked()), this, SLOT(new_profile_clicked()));

	delCurrentProfileButton.setIcon(QIcon(":/resources/minus.svg"));
	delCurrentProfileButton.setMaximumWidth(30);
	connect(&delCurrentProfileButton, SIGNAL(clicked()), this, SLOT(del_curr_profile_clicked()));

	profileSelectorLayout.addWidget(&profileList);
	profileSelectorLayout.addWidget(&addProfileButton);
	profileSelectorLayout.addWidget(&delCurrentProfileButton);
	profileSelectorBox.setLayout(&profileSelectorLayout);
}

void
ProfileSelectorWidget::updateComboBox()
{
	QStringList list=(QStringList()<<"red"<<"yello"<<"blue");
	profileList.addItems(list);
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

void
ProfileSelectorWidget::apply_dvc()
{
	std::map<int,int> std_map = dvc_map.toStdMap();
	nv.set_vibrance(&std_map);
	std_map = nv.get_vibrance();
	for(auto &memb : std_map)
		qDebug() << memb.first << "=" << memb.second;
}

AppProfile::AppProfile(Config &cfg, NVIDIA &nv, const QString name, ProfileSelectorWidget *p) :
	PSW(p),	// pointer to master PSW (has NV object)
	name(name), // profile name (or "default")
	cfg(cfg) // config file object ptr
{
	QMap<int,int> dvc_map = cfg.query_dvc(name);
	if(dvc_map.empty())
	{
		qDebug() << dvc_map;
		dvc_map = QMap(nv.get_vibrance());
		qDebug() << dvc_map;
		cfg.set_dvc(name, dvc_map);
	}

	QMap<int,int>::const_iterator i = dvc_map.constBegin();
	while(i != dvc_map.constEnd())
	{
		vlayout.addWidget(new DVCEntry(i.key(), dvc_map[i.key()], this));
		i++;
	}

	this->setLayout(&vlayout);
	this->setStyleSheet("border:none");
}

DVCEntry::DVCEntry(int dpyId, int dvc_level, AppProfile *p) :
	dpyId(dpyId),	// monitor id
	dvc(dvc_level),	// reference to NVIDIA std::map
	dvc_slider(Qt::Horizontal),
	dpy_name("DPY-" + QString::number(dpyId)),
	AP(p)	// reference to parent AppProfile widget
{
	dvc_slider.setMinimum(-100);
	dvc_slider.setMaximum(100);
	dvc_slider.setValue(dvc);

	dvc_le.setText(QString::number(dvc));
	dvc_le.setReadOnly(true);
	dvc_le.setMaximumWidth(30);

	connect(&dvc_slider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(onDVCSliderChanged(int)));

	hlayout.addWidget(&dpy_name);
	hlayout.addWidget(&dvc_slider);
	hlayout.addWidget(&dvc_le);
	this->setLayout(&hlayout);
}

void
DVCEntry::onDVCSliderChanged(int value)
{
	dvc_le.setText(QString::number(dvc_slider.sliderPosition()));
	dvc = value;

	// if we have default tab, apply values immediatelly
	if(AP->name.compare("default") == 0)
	{
		AP->PSW->dvc_map[dpyId] = value;
		AP->PSW->apply_dvc();
	}

	// update config
	QMap<int,int> cfg_dvc_map = AP->cfg.query_dvc(AP->name);
	cfg_dvc_map[dpyId] = value;
	AP->PSW->cfg.set_dvc(AP->name, cfg_dvc_map);
}
